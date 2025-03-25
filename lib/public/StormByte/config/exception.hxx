#pragma once

#include <StormByte/exception.hxx>
#include <StormByte/config/visibility.h>

/**
 * @namespace Config
 * @brief All the classes for handling configuration files and items
 */
namespace StormByte::Config {
	/**
	 * @class Exception
	 * @brief Base class for Config exceptions
	 */
	class STORMBYTE_CONFIG_PUBLIC Exception: public StormByte::Exception {
		public:
			/**
			 * Constructor
			 */
			using StormByte::Exception::Exception;	
	};

	/**
	 * @class WrongValueTypeConversion
	 * @brief Exception thrown when value type requested is not correct
	 */
	class STORMBYTE_CONFIG_PUBLIC WrongValueTypeConversion final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class ValueFailure
	 * @brief Exception thrown when setting a value to the wrong type
	 */
	class STORMBYTE_CONFIG_PUBLIC ValueFailure final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class InvalidName
	 * @brief Exception thrown when adding to a group an item with invalid name
	 */
	class STORMBYTE_CONFIG_PUBLIC InvalidName final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class InvalidPath
	 * @brief Exception thrown when adding to a group an item with invalid path
	 */
	class STORMBYTE_CONFIG_PUBLIC InvalidPath final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class ParseError
	 * @brief Thrown when a parser error is found when parsing File
	 */
	class STORMBYTE_CONFIG_PUBLIC ParseError final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class ItemNotFound
	 * @brief Exception thrown lookup path fails
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemNotFound final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class ItemAlreadyExists
	 * @brief Exception thrown when adding an item which already exists
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemAlreadyExists final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class ItemNameAlreadyExists
	 * @brief Exception thrown when adding an item which name already exists
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemNameAlreadyExists final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};

	/**
	 * @class OutOfBounds
	 * @brief Exception thrown when index is out of bounds
	 */
	class STORMBYTE_CONFIG_PUBLIC OutOfBounds final: public Exception {
		public:
			/**
			 * Constructor
			 */
			using Exception::Exception;
	};
}