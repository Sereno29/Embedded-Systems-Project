include(LibFindMacros)

# Dependencies
#libfind_package(udev)


# Use pkg-config to get hints about paths
libfind_pkg_check_modules(udev_PKGCONF)

# Include dir
find_path(udev_INCLUDE_DIR
  NAMES libudev.h
  PATHS ${udev_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(udev_LIBRARY
  NAMES udev
  PATHS ${udev_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(udev_PROCESS_INCLUDES udev_INCLUDE_DIR)
set(udev_PROCESS_LIBS udev_LIBRARY)
libfind_process(udev)
