##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
# import sys
# if sys.version_info.major < 3:
#     print("*** ins_nav is written for python version > 3.6 ***")

from ins_nav.ahrs import AHRS
from ins_nav.compass import TiltCompensatedCompass
# from ins_nav.errors import calc_errors

__author__ = 'Kevin J. Walchko'
__license__ = 'MIT'

try:
    from importlib.metadata import version # type: ignore
except ImportError:
    from importlib_metadata import version # type: ignore

__version__ = version("ins_nav")
