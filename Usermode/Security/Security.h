// ----------------------------------------------------------------------------
// Copyright (c) 2023 .ni0h. All rights reserved.
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not claim
//    that you wrote the original software. If you use this software in a
//    product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
// ----------------------------------------------------------------------------

// Security.h

// Helper function to read a key from JSON
std::string ReadKeyFromJson(const std::string& filename, const std::string& key) {
    std::ifstream i(filename);
    nlohmann::json j;
    i >> j;
    return j.value(key, ""); // Use the value() function to get the key with a default return
}

// Helper function to write a key to JSON
void WriteKeyToJson(const std::string& filename, const std::string& key, const std::string& value) {
    nlohmann::json j;
    j[key] = value;
    std::ofstream o(filename);
    o << j.dump(4); // Write JSON to file with 4 spaces indentation
}


bool validateKey(const std::string& key) {
    // Implement your key validation logic here
    // For demonstration, let's assume any non-empty key is valid
    return !key.empty();
}