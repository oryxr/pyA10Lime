/*
 * pyA20.c
 *
 * Copyright 2013 Stanimir Petev <support@olimex.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "Python.h"
#include "gpio_lib.h"


static PyObject *inp;
static PyObject *out;
static PyObject *high;
static PyObject *low;


typedef struct _gpio {
    char name[10];
    int ext;
    int pin;
    int con;
} gpio;

gpio pins[] = {
    /* GPIO-1 */
    {"PG0",     1,  SUNXI_GPG(0),   5},
    {"PG1",     1,  SUNXI_GPG(1),   7},
    {"PG2",     1,  SUNXI_GPG(2),   9},
    {"PG3",     1,  SUNXI_GPG(3),   11},
    {"PG4",     1,  SUNXI_GPG(4),   13},
    {"PG5",     1,  SUNXI_GPG(5),   15},
    {"PG6",     1,  SUNXI_GPG(6),   17},
    {"PG7",     1,  SUNXI_GPG(7),   19},
    {"PG8",     1,  SUNXI_GPG(8),   21},
    {"PG9",     1,  SUNXI_GPG(9),   23},
    {"PG10",    1,  SUNXI_GPG(10),  25},
    {"PG11",    1,  SUNXI_GPG(11),  27},
    {"PC3",     1,  SUNXI_GPC(3),   29},
    {"PC18",    1,  SUNXI_GPC(18),  31},
    {"PC19",    1,  SUNXI_GPC(19),  33},
    {"PC20",    1,  SUNXI_GPC(20),  35},
    {"PC21",    1,  SUNXI_GPC(21),  37},
    {"PC22",    1,  SUNXI_GPC(22),  39},
    {"PC23",    1,  SUNXI_GPC(23),  40},
    {"PC24",    1,  SUNXI_GPC(24),  38},
    {"PB18",    1,  SUNXI_GPB(18),  36},
    {"PB19",    1,  SUNXI_GPB(19),  34},
    {"PB20",    1,  SUNXI_GPB(20),  32},
    {"PB21",    1,  SUNXI_GPB(21),  30},

    /* GPIO-2 */
    {"PI0",     2,  SUNXI_GPI(0),   9},
    {"PI1",     2,  SUNXI_GPI(1),   11},
    {"PI2",     2,  SUNXI_GPI(2),   13},
    {"PI3",     2,  SUNXI_GPI(3),   15},
    {"PI4",     2,  SUNXI_GPI(4),   17},
    {"PI5",     2,  SUNXI_GPI(5),   19},
    {"PI6",     2,  SUNXI_GPI(6),   21},
    {"PI7",     2,  SUNXI_GPI(7),   23},
    {"PI8",     2,  SUNXI_GPI(8),   25},
    {"PI9",     2,  SUNXI_GPI(9),   27},
    {"PI10",    2,  SUNXI_GPI(10),  29},
    {"PI11",    2,  SUNXI_GPI(11),  31},
    {"PI12",    2,  SUNXI_GPI(12),  33},
    {"PI13",    2,  SUNXI_GPI(13),  35},
    {"PI14",    2,  SUNXI_GPI(14),  37},
    {"PI15",    2,  SUNXI_GPI(15),  39},
    {"PI16",    2,  SUNXI_GPI(16),  40},
    {"PI17",    2,  SUNXI_GPI(17),  38},
    {"PI18",    2,  SUNXI_GPI(18),  36},
    {"PI19",    2,  SUNXI_GPI(19),  34},
    {"PI20",    2,  SUNXI_GPI(20),  32},
    {"PI21",    2,  SUNXI_GPI(21),  30},
    {"PE0",     2,  SUNXI_GPE(0),   6},
    {"PE1",     2,  SUNXI_GPE(1),   8},
    {"PE2",     2,  SUNXI_GPE(2),   10},
    {"PE3",     2,  SUNXI_GPE(3),   12},
    {"PE4",     2,  SUNXI_GPE(4),   14},
    {"PE5",     2,  SUNXI_GPE(5),   16},
    {"PE6",     2,  SUNXI_GPE(6),   18},
    {"PE7",     2,  SUNXI_GPE(7),   20},
    {"PE8",     2,  SUNXI_GPE(8),   22},
    {"PE9",     2,  SUNXI_GPE(9),   24},
    {"PE10",    2,  SUNXI_GPE(10),  26},
    {"PE11",    2,  SUNXI_GPE(11),  28},

    /* GPIO-3 */
    {"PH0",     3,  SUNXI_GPH(0),   7},
    {"PH7",     3,  SUNXI_GPH(7),   9},
    {"PH9",     3,  SUNXI_GPH(9),   11},
    {"PH10",    3,  SUNXI_GPH(10),  13},
    {"PH11",    3,  SUNXI_GPH(11),  15},
    {"PH12",    3,  SUNXI_GPH(12),  17},
    {"PH13",    3,  SUNXI_GPH(13),  19},
    {"PH14",    3,  SUNXI_GPH(14),  21},
    {"PH15",    3,  SUNXI_GPH(15),  23},
    {"PH16",    3,  SUNXI_GPH(16),  25},
    {"PH17",    3,  SUNXI_GPH(17),  27},
    {"PH18",    3,  SUNXI_GPH(18),  29},
    {"PH19",    3,  SUNXI_GPH(19),  31},
    {"PH20",    3,  SUNXI_GPH(20),  33},
    {"PH21",    3,  SUNXI_GPH(21),  35},
    {"PH22",    3,  SUNXI_GPH(22),  37},
    {"PH23",    3,  SUNXI_GPH(23),  39},
    {"PH24",    3,  SUNXI_GPH(24),  34},
    {"PH25",    3,  SUNXI_GPH(25),  36},
    {"PH26",    3,  SUNXI_GPH(26),  38},
    {"PH27",    3,  SUNXI_GPH(27),  40},
    {"PB3",     3,  SUNXI_GPB(3),   6},
    {"PB4",     3,  SUNXI_GPB(4),   8},
    {"PB5",     3,  SUNXI_GPB(5),   10},
    {"PB6",     3,  SUNXI_GPB(6),   12},
    {"PB7",     3,  SUNXI_GPB(7),   14},
    {"PB8",     3,  SUNXI_GPB(8),   16},
    {"PB10",    3,  SUNXI_GPB(10),  18},
    {"PB11",    3,  SUNXI_GPB(11),  20},
    {"PB12",    3,  SUNXI_GPB(12),  22},
    {"PB13",    3,  SUNXI_GPB(13),  24},
    {"PB14",    3,  SUNXI_GPB(14),  26},
    {"PB15",    3,  SUNXI_GPB(15),  28},
    {"PB16",    3,  SUNXI_GPB(16),  30},
    {"PB17",    3,  SUNXI_GPB(17),  32},

    /* LED */
    {"PH2",     0,  SUNXI_GPH(2),   0},

    /* GPIO-4 */
    {"PC7",     4,  SUNXI_GPC(7),   16},
    {"PC16",     4,  SUNXI_GPC(16), 18},
    {"PC17",     4,  SUNXI_GPC(17), 20}



};







static PyObject* py_output(PyObject* self, PyObject* args) {
    int gpio;
    int value;

    if(!PyArg_ParseTuple(args, "ii", &gpio, &value))
        return NULL;

    if(value != 0 && value != 1) {
        PyErr_SetString(PyExc_IOError, "Invalid output state");
        return NULL;
    }

    if(sunxi_gpio_get_cfgpin(gpio) != SUNXI_GPIO_OUTPUT) {
        PyErr_SetString(PyExc_IOError, "GPIO is no an output");
        return NULL;
    }
    sunxi_gpio_output(gpio, value);

    Py_RETURN_NONE;
}
static PyObject* py_input(PyObject* self, PyObject* args) {
    int gpio;
    int result;

    if(!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;

    if(sunxi_gpio_get_cfgpin(gpio) != SUNXI_GPIO_INPUT) {
        PyErr_SetString(PyExc_IOError, "GPIO is not an input");
        return NULL;
    }
    result = sunxi_gpio_input(gpio);

    if(result == -1) {
        PyErr_SetString(PyExc_IOError, "Reading pin failed");
        return NULL;
    }


    return Py_BuildValue("i", result);
}

static PyObject* py_setcfg(PyObject* self, PyObject* args) {
    int gpio;
    int direction;

    if(!PyArg_ParseTuple(args, "ii", &gpio, &direction))
        return NULL;

    if(direction != 0 && direction != 1 && direction != 2) {
        PyErr_SetString(PyExc_IOError, "Invalid direction");
        return NULL;
    }
    sunxi_gpio_set_cfgpin(gpio, direction);

    Py_RETURN_NONE;
}
static PyObject* py_getcfg(PyObject* self, PyObject* args) {
    int gpio;
    int result;


    if(!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;

    result = sunxi_gpio_get_cfgpin(gpio);


    return Py_BuildValue("i", result);


}
static PyObject* py_init(PyObject* self, PyObject* args) {

    int result;

    result = sunxi_gpio_init();
    if(result == -1) {
        PyErr_SetString(PyExc_IOError, "No access to /dev/mem. Try running as root!");
        return NULL;
    }

    Py_RETURN_NONE;
}
static PyObject* py_cleanup(PyObject* self, PyObject* args) {

    sunxi_gpio_cleanup();
    Py_RETURN_NONE;
}

static PyObject* py_info(PyObject* self, PyObject* args){
    int gpio;
    int i;

    if(!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;


    for(i = 0; i < sizeof(pins)/sizeof(gpio); i++){
        if (pins[i].pin == gpio){
            return Py_BuildValue("{s:s, s:i, s:i, s:i}",
                "name", pins[i].name,
                "connector", pins[i].ext,
                "pin", pins[i].con,
                "value", pins[i].pin);
        }
    }

    Py_RETURN_NONE;
}


PyMethodDef module_methods[] = {
    {"init", py_init, METH_NOARGS, "Initialize module"},
    {"cleanup", py_cleanup, METH_NOARGS, "munmap /dev/map."},
    {"setcfg", py_setcfg, METH_VARARGS, "Set direction."},
    {"getcfg", py_getcfg, METH_VARARGS, "Get direction."},
    {"output", py_output, METH_VARARGS, "Set output state"},
    {"input", py_input, METH_VARARGS, "Get input state"},
    {"info", py_info, METH_VARARGS, "Get detailed info"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION>=3
  #define MOD_ERROR_VAL NULL
  #define MOD_SUCCESS_VAL(val) val
  #define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
  #define MOD_DEF(obj, name, doc, methods) \
    static struct PyModuleDef module_def = { \
      PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
    obj = PyModule_Create(&module_def);
#else
  #define MOD_ERROR_VAL
  #define MOD_SUCCESS_VAL(val)
  #define MOD_INIT(name) void init##name(void)
  #define MOD_DEF(obj, name, doc, methods) \
          ob = Py_InitModule(name, methods, doc);
//      "A10Lime module", /* m_name */
//      NULL,             /* m_doc */
//      -1,               /* m_size */
//    module_methods,   /* m_methods */
//    NULL,             /* m_reload */
//    NULL,             /* m_traverse */
//    NULL,             /* m_clear */
//    NULL,             /* m_free */
};
#endif


MOD_INIT(A10Lime) {
    PyObject* module;

    MOD_DEF(module, "A10Lime", NULL, module_methods)
//#if PY_MAJOR_VERSION >= 3
//    module = PyModule_Create(&module_def);
//#else
//    module = Py_InitModule("A10Lime", module_methods);
//#endif


    if(module == NULL)
      return MOD_ERROR_VAL;

//#if PY_MAJOR_VERSION >= 3
//return module;
//#else
//return;
//#endif


    high = Py_BuildValue("i", HIGH);
    PyModule_AddObject(module, "HIGH", high);

    low = Py_BuildValue("i", LOW);
    PyModule_AddObject(module, "LOW", low);

    inp = Py_BuildValue("i", INPUT);
    PyModule_AddObject(module, "INPUT", inp);

    out = Py_BuildValue("i", OUTPUT);
    PyModule_AddObject(module, "OUTPUT", out);


    int i;
    for (i = 0; i < sizeof(pins)/sizeof(gpio); i++){
        if(strcmp(pins[i].name, "PH2") ){
            char name[10];
            sprintf(name, "PIN%d_%d", pins[i].ext, pins[i].con);
            PyModule_AddObject(module, name, Py_BuildValue("i", pins[i].pin));
            }
        PyModule_AddObject(module, pins[i].name, Py_BuildValue("i", pins[i].pin));
    }



    if(Py_AtExit(sunxi_gpio_cleanup) != 0)//{
        sunxi_gpio_cleanup();

    return MOD_SUCCESS_VAL(module);
//#if PY_MAJOR_VERSION >= 3
//        return NULL;
//#else
//        return;
//#endif
   // }



}

#ifdef __cplusplus
} // extern "C"
#endif
