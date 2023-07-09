##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from .nav_frames import NavigationFrame
from .wgs84 import WGS84
from .filters.madgwick import Madgwick
from .filters.mahony import Mahony

from importlib.metadata import version # type: ignore

__author__ = 'Kevin J. Walchko'
__license__ = 'MIT'
__version__ = version("ins_nav")
