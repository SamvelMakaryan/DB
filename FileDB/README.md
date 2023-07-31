# FileDB

FileDB is a simple file-based database program that allows users to store and manage structured data in files. It provides an easy-to-use interface to create, read, update, and delete data entries, acting as a lightweight database solution for various applications.

## Commands

This system supports following commands`

1. `create`
    - creates new object with given parameters.
    - example: create User name age
2. `insert`
    - inserts new value to existing objects.
    - example: insert User name=A age=1
3. `delete`
    - deletes existing value(s).
    - example 1: delete User
    - example 2: delete User id=5
    - example 3: delete User age=1
4. `update`
    - updates existing values using id.
    - example: update id=1 name=B
5. `display`
    - displays given object.
    - example: display User
6. `exit`
    - exits program saving all info.
    - example: exit

## Requirements

- C++ compiler.
- CMake (can be compiled directly).

## Usage

- clone this repository.
- move to 'FileDB' directory.
- create 'build' directory and move there.
- give cmake configurations (cmake ..).
- build program (make).
- run FileDB executable.



