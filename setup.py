from distutils.core import setup, Extension


setup(
    name                =   'pyA10Lime',
    version             = '0.1.2',
    author              = 'Stefan Mavrodiev',
    author_email        = 'support@olimex.com',
    url                 = 'https://www.olimex.com/',
    license             = 'MIT',
    description         = 'Control GPIOs on A10-Olinuxino-Lime.',
    long_description    = open('README.txt').read() + open('CHANGES.txt').read(),
    classifiers         = [ 'Development Status :: 3 - Alpha',
                            'Environment :: Console',
                            'Intended Audience :: Developers',
                            'Intended Audience :: Education',
                            'License :: OSI Approved :: MIT License',
                            'Operating System :: POSIX :: Linux',
                            'Programming Language :: Python',
                            'Topic :: Home Automation',
                            'Topic :: Software Development :: Embedded Systems'
          ],
    ext_modules         = [Extension('A10Lime', ['source/gpio_lib.c', 'source/pyA10Lime.c'])],
    package_dir={'': 'source'},
    packages=[''],
)
