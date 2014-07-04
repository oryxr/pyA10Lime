This package provides class to control the GPIO on A10-OLinuXino-Lime
Current release does no support any peripheral functions.

Example
=======

Typical usage::

    >>>import A10Lime as lime

    # Initialize module
    >>>lime.init()

    # Read configuration of GPIO
    >>>lime.getcfg(lime.PC3)
    # or you can use
    >>>lime.getcfg(lime.PIN1_29)

    # You can address GPIO either by pin numeration or port name
    # For example PG0 pin on GPIO-1 connector can be accessed with lime.PG0 or lime.PIN1_5

    # Set configuration
    >>>lime.setcfg(lime.PC3, lime.OUTPUT)
    >>>lime.setcfg(lime.PC3, lime.INPUT)

    # Set output level
    >>>lime.output(lime.PC3, lime.HIGH)
    >>>lime.output(lime.PC3, lime.LOW)

    # Read input state
    >>>lime.input(lime.PC3)

    # Read detailed info about pin
    >>>lime.info(lime.PC3)
    >>>lime.info(lime.PIN1_29)

Warning
=======

    Before using this tool it is HIGHLY RECOMENDED to check A10-Olinuxino-Lime schematic.

