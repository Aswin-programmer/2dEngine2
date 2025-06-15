#pragma once

class LuaLogBinding
{
public:
	static void RegisterLuaLogBinding( sol::state& lua )
	{
		// Register LUA_INFO
		lua["LUA_INFO"] = []( const std::string& message, sol::variadic_args args ) {
			std::ostringstream oss;
			oss << message; // Start with the base message

			// Append any additional arguments
			for ( auto it = args.begin(); it != args.end(); ++it )
			{
				oss << " "; // Add spacing between arguments
				if ( it->is<double>() )
					oss << it->as<double>();
				else if ( it->is<std::string>() )
					oss << it->as<std::string>();
				else if ( it->is<bool>() )
					oss << ( it->as<bool>() ? "true" : "false" );
				else
					oss << "<?>";
			}

			LUA_INFO( "{}", oss.str() );
		};

		// Register LUA_WARN
		lua["LUA_WARN"] = []( const std::string& message, sol::variadic_args args ) {
			std::ostringstream oss;
			oss << message; // Start with the base message

			// Append any additional arguments
			for ( auto it = args.begin(); it != args.end(); ++it )
			{
				oss << " "; // Add spacing between arguments
				if ( it->is<double>() )
					oss << it->as<double>();
				else if ( it->is<std::string>() )
					oss << it->as<std::string>();
				else if ( it->is<bool>() )
					oss << ( it->as<bool>() ? "true" : "false" );
				else
					oss << "<?>";
			}

			LUA_WARN( "{}", oss.str() );
		};

		// Register LUA_ERROR
		lua["LUA_ERROR"] = []( const std::string& message, sol::variadic_args args ) {
			std::ostringstream oss;
			oss << message; // Start with the base message

			// Append any additional arguments
			for ( auto it = args.begin(); it != args.end(); ++it )
			{
				oss << " "; // Add spacing between arguments
				if ( it->is<double>() )
					oss << it->as<double>();
				else if ( it->is<std::string>() )
					oss << it->as<std::string>();
				else if ( it->is<bool>() )
					oss << ( it->as<bool>() ? "true" : "false" );
				else
					oss << "<?>";
			}

			LUA_ERROR( "{}", oss.str() );
		};
	};
};
