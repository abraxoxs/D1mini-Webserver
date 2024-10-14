// converter.h

#ifndef CONVERTER_H
#define CONVERTER_H

/**
 * @brief Converts a string to a boolean value.
 * 
 * This function takes a string as input and returns a boolean value.
 * If the input string is "true", it returns true. For any other input,
 * it returns false.
 * 
 * @param str The input string to be converted to a boolean.
 * @return true if the input string is "true", false otherwise.
 */
bool stringToBool(String str) {
    if (str == "true") {
        return true;
    } else {
        return false;
    }
}

#endif

