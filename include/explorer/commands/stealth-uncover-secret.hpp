/**
 * Copyright (c) 2011-2014 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin_explorer.
 *
 * libbitcoin_explorer is free software: you can redistribute it and/or
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
#ifndef BX_STEALTH_UNCOVER_SECRET_HPP
#define BX_STEALTH_UNCOVER_SECRET_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <bitcoin/bitcoin.hpp>
#include <explorer/command.hpp>
#include <explorer/define.hpp>
#include <explorer/generated.hpp>
#include <explorer/primitives/address.hpp>
#include <explorer/primitives/base16.hpp>
#include <explorer/primitives/base58.hpp>
#include <explorer/primitives/btc.hpp>
#include <explorer/primitives/btc160.hpp>
#include <explorer/primitives/btc256.hpp>
#include <explorer/primitives/ec_private.hpp>
#include <explorer/primitives/ec_public.hpp>
#include <explorer/primitives/encoding.hpp>
#include <explorer/primitives/hashtype.hpp>
#include <explorer/primitives/hd_key.hpp>
#include <explorer/primitives/hd_priv.hpp>
#include <explorer/primitives/hd_pub.hpp>
#include <explorer/primitives/header.hpp>
#include <explorer/primitives/input.hpp>
#include <explorer/primitives/output.hpp>
#include <explorer/primitives/prefix.hpp>
#include <explorer/primitives/raw.hpp>
#include <explorer/primitives/script.hpp>
#include <explorer/primitives/stealth.hpp>
#include <explorer/primitives/transaction.hpp>
#include <explorer/primitives/wif.hpp>
#include <explorer/primitives/wrapper.hpp>
#include <explorer/utility/compat.hpp>
#include <explorer/utility/config.hpp>
#include <explorer/utility/utility.hpp>

/********* GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY **********/

namespace explorer {
namespace commands {

/**
 * Class to implement the stealth-uncover-secret command.
 */
class stealth_uncover_secret 
    : public command
{
public:

    /**
     * The symbolic (not localizable) command name, lower case.
     */
    static const char* symbol() { return "stealth-uncover-secret"; }

    /**
     * The member symbolic (not localizable) command name, lower case.
     */
    virtual const char* name()
    {
        return stealth_uncover_secret::symbol();
    }

    /**
     * The localizable command category name, upper case.
     */
    virtual const char* category()
    {
        return "STEALTH";
    }

    /**
     * Load program argument definitions.
     * A value of -1 indicates that the number of instances is unlimited.
     * @return  The loaded program argument definitions.
     */
    virtual arguments_metadata& load_arguments()
    {
        return get_argument_metadata()
            .add("EPHEMERAL_PUBKEY", 1)
            .add("SCAN_SECRET", 1)
            .add("SPEND_SECRET", 1);
    }
	
	/**
     * Load parameter fallbacks from file or input as appropriate.
     * @param[in]  input  The input stream for loading the parameters.
     * @param[in]         The loaded variables.
     */
    virtual void load_fallbacks(std::istream& input, 
        po::variables_map& variables)
    {
    }
    
    /**
     * Load program option definitions.
     * The implicit_value call allows flags to be strongly-typed on read while
     * allowing but not requiring a value on the command line for the option.
     * BUGBUG: see boost bug/fix: svn.boost.org/trac/boost/ticket/8009
     * @return  The loaded program option definitions.
     */
    virtual options_metadata& load_options()
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
            (
                BX_VARIABLE_CONFIG ",c",
                value<boost::filesystem::path>(),                 
                "The path and file name for the configuration settings file for this application."
            )
            (
                "help,h",
                value<bool>(&option_.help)->implicit_value(true),
                "Derive the stealth private key necessary to spend a stealth payment."
            )
            (
                "EPHEMERAL_PUBKEY",
                value<primitives::ec_public>(&argument_.ephemeral_pubkey),
                "The Base16 ephemeral EC public key retrieved from the stealth payment metadata."
            )
            (
                "SCAN_SECRET",
                value<primitives::ec_private>(&argument_.scan_secret),
                "The Base16 EC private key corresponding to the public key required to generate a stealth address."
            )
            (
                "SPEND_SECRET",
                value<primitives::ec_private>(&argument_.spend_secret)->required(),
                "A Base16 EC private key that can spend payments to the stealth address."
            );

        return options;
    }

    /**
     * Invoke the command.
     * @param[out]  output  The input stream for the command execution.
     * @param[out]  error   The input stream for the command execution.
     * @return              The appropriate console return code { -1, 0, 1 }.
     */
    virtual console_result invoke(std::ostream& output, std::ostream& cerr);
        
    /* Properties */

    /**
     * Get the value of the EPHEMERAL_PUBKEY argument.
     */
    virtual primitives::ec_public& get_ephemeral_pubkey_argument()
    {
        return argument_.ephemeral_pubkey;
    }
    
    /**
     * Set the value of the EPHEMERAL_PUBKEY argument.
     */
    virtual void set_ephemeral_pubkey_argument(
        const primitives::ec_public& value)
    {
        argument_.ephemeral_pubkey = value;
    }

    /**
     * Get the value of the SCAN_SECRET argument.
     */
    virtual primitives::ec_private& get_scan_secret_argument()
    {
        return argument_.scan_secret;
    }
    
    /**
     * Set the value of the SCAN_SECRET argument.
     */
    virtual void set_scan_secret_argument(
        const primitives::ec_private& value)
    {
        argument_.scan_secret = value;
    }

    /**
     * Get the value of the SPEND_SECRET argument.
     */
    virtual primitives::ec_private& get_spend_secret_argument()
    {
        return argument_.spend_secret;
    }
    
    /**
     * Set the value of the SPEND_SECRET argument.
     */
    virtual void set_spend_secret_argument(
        const primitives::ec_private& value)
    {
        argument_.spend_secret = value;
    }

    /**
     * Get the value of the help option.
     */
    virtual bool& get_help_option()
    {
        return option_.help;
    }
    
    /**
     * Set the value of the help option.
     */
    virtual void set_help_option(
        const bool& value)
    {
        option_.help = value;
    }

private:

    /**
     * Command line argument bound variables.
     * Uses cross-compiler safe constructor-based zeroize.
     * Zeroize for unit test consistency with program_options initialization.
     */
    struct argument
    {
        argument()
          : ephemeral_pubkey(),
            scan_secret(),
            spend_secret()
        {
        }
        
        primitives::ec_public ephemeral_pubkey;
        primitives::ec_private scan_secret;
        primitives::ec_private spend_secret;
    } argument_;
    
    /**
     * Command line option bound variables.
     * Uses cross-compiler safe constructor-based zeroize.
     * Zeroize for unit test consistency with program_options initialization.
     */
    struct option
    {
        option()
          : help()
        {
        }
        
        bool help;
    } option_;
};

} // commands
} // explorer

#endif