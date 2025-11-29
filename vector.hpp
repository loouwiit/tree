#pragma once

#include <SFML/System.hpp>

template <class T>
constexpr static T operator*(sf::Vector3<T> a, sf::Vector3<T>b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

template <class T>
constexpr static T abs2(sf::Vector3<T> v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

template <class T>
constexpr static T abs2(sf::Vector2<T> v) { return v.x * v.x + v.y * v.y; }

template <class T>
constexpr static T abs(sf::Vector3<T> v) { return std::sqrt(abs2(v)); }

template <class T>
constexpr static T abs(sf::Vector2<T> v) { return std::sqrt(abs2(v)); }
