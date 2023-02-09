
#ifndef __inc_directions_H__
#define __inc_directions_H__

#include <iostream>

enum class Direction { UP, DOWN, LEFT, RIGHT };

std::ostream &operator<<(std::ostream &os, const Direction &dir);
std::string to_string(const Direction &dir);

Direction opposite(const Direction &dir);

Direction turn_left(const Direction &dir);
Direction turn_right(const Direction &dir);

#endif // __inc_directions_H__
