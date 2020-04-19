##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################

from ins_nav.ahrs import AHRS
from ins_nav.compass import TiltCompensatedCompass

try:
    from importlib.metadata import version # type: ignore
except ImportError:
    from importlib_metadata import version # type: ignore

__author__ = 'Kevin J. Walchko'
__license__ = 'MIT'
__version__ = version("ins_nav")
