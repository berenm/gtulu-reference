======================================================
gtulu-reference — opengl reference C++ wrapper library
======================================================
.. image:: https://travis-ci.org/berenm/gtulu-reference.png?branch=master
    :alt: Build Status
    :target: https://travis-ci.org/berenm/gtulu-reference

GOAL OF GTULU
```````````````

1. **Track the OpenGL specifications and API evolutions**, keeping track of the functions and constants additions or removals. Tracking inconsistencies and correcting the manpages.
2. **Provide a programmable way to generate headers and libraries** corresponding to a specific version of OpenGL.
3. **Provide a stable API layer for OpenGL programming**, without having to rely on vendor-dependent OpenGL libraries with heterogeneous symbol availability. Targetting a specific version of OpenGL would simply mean linking against a specific version of such library set, disregarding of the availability of the vendor's symbols at the compilation time.
4. **Provide a modern C++-style API**, using function overloading, namespaces, strong type checks and datastructures, to simplify the OpenGL API and improve the interoperability with the C++ standard library. 
5. **Provide debugging information** around OpenGL API calls. Optionnally logging each call in a human-friendly format, warning of vendor's not implemented function calls, and automatically checking OpenGL errors and logging them and/or reporting them as exceptions.
6. **Decouple from** libgtulu_ library, which would use such API as a base layer and add template constraints around it.

.. _libgtulu: https://github.com/berenm/libgtulu

BUILDING
```````````````
.. code:: bash

  mkdir build
  cd build
  cmake ..
  make

COPYING INFORMATION
`````````````````````
*Except from the man pages and registry headers, which are property of OpenGL and Khronos group and under specific copyright*

 Distributed under the Boost Software License, Version 1.0.

 See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
