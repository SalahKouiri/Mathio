# Mathio: The Advanced Scientific Calculator

Mathio is an advanced scientific calculator designed to assist users in performing complex mathematical calculations with ease. This project aims to provide a robust and user-friendly interface for both basic and advanced mathematical operations.

## Features

- **Basic Arithmetic Operations**: Supports addition, subtraction, multiplication, and division.
- **Expression Parsing**: Utilizes the Shunting Yard Algorithm to convert infix expressions to postfix notation for accurate calculations.
- **User Input Handling**: Allows users to input mathematical expressions and receive instant results.
- **Modular Design**: The project is structured into multiple source files for better organization and maintainability.

## Project Structure

```
Mathio
├── src
│   ├── main.c            # Entry point of the application
│   ├── calculator.c      # Implementation of arithmetic operations
│   ├── calculator.h      # Header file for calculator functions
│   ├── parser.c          # Expression parsing logic
│   ├── parser.h          # Header file for parser functions
│   └── utils
│       ├── utils.c       # Utility functions for the calculator
│       └── utils.h       # Header file for utility functions
├── include
│   └── mathio.h          # Main interface for the Mathio project
├── tests
│   ├── test_calculator.c  # Unit tests for calculator functions
│   ├── test_parser.c      # Unit tests for parser functions
│   └── test_utils.c       # Unit tests for utility functions
├── Makefile               # Build instructions for the project
└── README.md              # Documentation for the project
```

## Installation

To build the project, ensure you have a C compiler installed. Navigate to the project directory and run:

```
make
```

This will compile the source files and create the executable.

## Usage

After building the project, run the executable to start using Mathio. You can input mathematical expressions directly, and the calculator will provide the results in real-time.

## Contributing

Contributions are welcome! If you would like to contribute to the Mathio project, please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.