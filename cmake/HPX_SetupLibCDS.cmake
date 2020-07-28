# Copyright (c) 2020      Weile Wei
# Copyright (c) 2020      John Biddiscombe
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# set(HPX_WITH_LIBCDS_GIT_REPOSITORY
# https://github.com/STEllAR-GROUP/libcds.git) set(HPX_WITH_LIBCDS_GIT_TAG
# hpx-module)
set(FETCHCONTENT_UPDATES_DISCONNECTED_libcds ON)

if(HPX_WITH_LIBCDS)
  include(FetchContent)
  include(HPX_Message)

  set(LIBCDS_GENERATE_SOURCELIST ON)

  set(LIBCDS_WITH_HPX ON)
  set(LIBCDS_AS_HPX_MODULE ON)

  hpx_info(
    "Fetching libCDS from repository: ${HPX_WITH_LIBCDS_GIT_REPOSITORY}, "
    "tag: ${HPX_WITH_LIBCDS_GIT_TAG}"
  )
  fetchcontent_declare(
    libcds
    GIT_REPOSITORY ${HPX_WITH_LIBCDS_GIT_REPOSITORY}
    GIT_TAG ${HPX_WITH_LIBCDS_GIT_TAG}
    GIT_SHALLOW TRUE
  )
  fetchcontent_getproperties(libcds)

  if(NOT libcds_POPULATED)
    fetchcontent_populate(libcds)
    set(LIBCDS_CXX_STANDARD ${HPX_CXX_STANDARD})
    add_subdirectory(${libcds_SOURCE_DIR} ${libcds_BINARY_DIR})
    list(TRANSFORM LIBCDS_SOURCELIST PREPEND "${libcds_SOURCE_DIR}/")
    set(LIBCDS_SOURCE_DIR ${libcds_SOURCE_DIR})
  endif()

endif()
