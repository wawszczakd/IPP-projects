# IPP Projects

This repository contains my projects from the course "Individual Programming
Project".

## Overview

IPP stands for "Indywidualny Projekt Programistyczny" in Polish, which
translates to "Individual Programming Project". This repository includes my
projects from this course. Both projects are implemented in C.

## Project Descriptions

Below is the list of short project descriptions:

1. **Labyrinth**: Develop a program that finds the shortest path in a
multi-dimensional maze. The labyrinth consists of filled and empty cubes, with
movement possible between adjacent empty cubes. The program reads input data,
including the labyrinth dimensions, starting and ending positions, and wall
configurations. It then determines the shortest path length from the starting
position to the ending position and outputs it. In case no path exists, it
prints "NO WAY."

2. **Phone Forward**: This project focuses on implementing operations on phone
numbers. It is divided into three parts:

   - Part 1: Implement the module for call forwarding operations. The module
   allows performing operations on phone numbers represented as strings
   consisting of digits `0` to `9`. Numbers can be forwarded, where forwarding
   involves finding the longest prefix of a number that has a forwarding rule
   and replacing that prefix with the appropriate characters.
   
   - Part 2: Modify the phone number representation to include two additional
   digits. Digits ten and eleven are represented as `*` and `#`, respectively.
   In this part, implement the `phfwdReverse` function according to the provided
   specification in the solution template from the first part.
   
   - Part 3: Finalize the phone number operations module. Implement the
   `phfwdGetReverse` function according to the provided specification. The
   function should return a lexicographically sorted list of all phone numbers
   that map to the given number using the `phfwdGet` function. The function
   should allocate and return a structure `PhoneNumbers`, which should be freed
   using the `phnumDelete` function.

Please refer to the respective project directories for detailed solutions.
