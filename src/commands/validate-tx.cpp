/**
 * Copyright (c) 2011-2014 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin-explorer.
 *
 * libbitcoin-explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <bitcoin/explorer/commands/validate-tx.hpp>

#include <iostream>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/explorer/callback_state.hpp>
#include <bitcoin/explorer/define.hpp>
#include <bitcoin/explorer/display.hpp>
#include <bitcoin/explorer/obelisk_client.hpp>
#include <bitcoin/explorer/prop_tree.hpp>
#include <bitcoin/explorer/utility/utility.hpp>

using namespace bc;
using namespace bc::client;
using namespace bc::explorer;
using namespace bc::explorer::commands;
using namespace bc::explorer::primitives;

static void handle_error(callback_state& state, const std::error_code& error)
{
    // BX_VALIDATE_TX_INVALID_INPUT is not currently utilized.
    // The client suppresses an index list which may have 0 or one element.
    // The list contains the index of the input which caused the failure.
    state.handle_error(error);
}

static void handle_callback(callback_state& state, size_t position,
    const index_list& indexes)
{
    if (indexes.empty())
    {
        state.output(BX_VALIDATE_TX_VALID);
        return;
    }
    
    const auto unconfirmed = join(numbers_to_strings(indexes), ", ");
    state.output(format(BX_VALIDATE_TX_UNCONFIRMED_INPUTS) % unconfirmed);
}

static void validate_tx_from_transaction(obelisk_client& client,
    callback_state& state, const primitives::transaction& transaction)
{
    auto on_done = [&state](const index_list& unconfirmed)
    {
        handle_callback(state, state, unconfirmed);
    };

    auto on_error = [&state](const std::error_code& error)
    {
        handle_error(state, error);
    };

    client.get_codec()->validate(on_error, on_done, transaction);
}

console_result validate_tx::invoke(std::ostream& output,
    std::ostream& error)
{
    // Bound parameters.
    const auto& transaction = get_transaction_argument();
    const auto retries = get_general_retries_setting();
    const auto timeout = get_general_wait_setting();
    const auto& server = if_else(get_general_network_setting() == "testnet",
        get_testnet_url_setting(), get_mainnet_url_setting());

    czmqpp::context context;
    obelisk_client client(context, period_ms(timeout), retries);

    if (client.connect(server) < 0)
    {
        display_connection_failure(error, server);
        return console_result::failure;
    }

    callback_state state(error, output);
    validate_tx_from_transaction(client, state, transaction);
    client.resolve_callbacks();

    return state.get_result();
}
