======================================================
gtulu-reference — opengl reference C++ wrapper library
======================================================
.. image:: https://secure.travis-ci.org/berenm/gtulu-reference.png?branch=master
    :alt: Build Status
    :target: https://travis-ci.org/berenm/gtulu-reference

PURPOSE OF GTULU
`````````````````````

The purposes of this project are:
# To track the OpenGL specifications and API evolutions, keeping track of the functions and constants additions or removals.
# Using the information provided by the former, to provide a programmable way to generate headers and libraries corresponding to a specific version of OpenGL.
# Using such generation, to provide a stable API layer for OpenGL programming, without having to rely on vendor-dependent OpenGL libraries with heterogeneous symbol availability. Targetting a specific version of OpenGL would simply mean linking against a specific version of such library set, disregarding of the availability of the vendor's symbols at the compilation time.
# Provide a C++-style API, using function overloading, namespaces, strong type checks and datastructures, to simplify the OpenGL API and improve the interoperability with the C++ standard library. 
# Provide more debugging information around OpenGL API calls. Optionnally logging each call in a human-friendly format, warning of vendor's not implemented function calls, and automatically checking OpenGL errors and logging them and/or reporting them as exceptions.


COPYING INFORMATION
`````````````````````

 Distributed under the Boost Software License, Version 1.0.

 See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
