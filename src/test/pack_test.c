/** !
 * pack tester
 * 
 * @file src/test/pack_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>
#include <core/test.h>

// preprocessor macros
/// keys
#define CHAR_A   ((char)   1)
#define CHAR_B   ((char)   2)
#define CHAR_C   ((char)   3)
#define SHORT_A  ((short)  1)
#define SHORT_B  ((short)  2)
#define SHORT_C  ((short)  3)
#define INT_A    ((int)    1)
#define INT_B    ((int)    2)
#define INT_C    ((int)    3)
#define LONG_A   ((long)   1)
#define LONG_B   ((long)   2)
#define LONG_C   ((long)   3)
#define FLOAT_A  ((float)  1)
#define FLOAT_B  ((float)  2)
#define FLOAT_C  ((float)  3)
#define DOUBLE_A ((double) 1)
#define DOUBLE_B ((double) 2)
#define DOUBLE_C ((double) 3)

/// test vectors
#define CHAR_1     ( (char  [1]) { CHAR_A                       } )
#define CHAR_2     ( (char  [1]) { CHAR_B                       } )
#define CHAR_3     ( (char  [1]) { CHAR_C                       } )
#define SHORT_1    ( (short [1]) { SHORT_A                      } )
#define SHORT_2    ( (short [1]) { SHORT_B                      } )
#define SHORT_3    ( (short [1]) { SHORT_C                      } )
#define INT_1      ( (int   [1]) { INT_A                        } )
#define INT_2      ( (int   [1]) { INT_B                        } )
#define INT_3      ( (int   [1]) { INT_C                        } )
#define LONG_1     ( (long  [1]) { LONG_A                       } )
#define LONG_2     ( (long  [1]) { LONG_B                       } )
#define LONG_3     ( (long  [1]) { LONG_C                       } )
#define FLOAT_1    ( (float [1]) { FLOAT_A                      } )
#define FLOAT_2    ( (float [1]) { FLOAT_B                      } )
#define FLOAT_3    ( (float [1]) { FLOAT_C                      } )
#define DOUBLE_1   ( (double[1]) { DOUBLE_A                     } )
#define DOUBLE_2   ( (double[1]) { DOUBLE_B                     } )
#define DOUBLE_3   ( (double[1]) { DOUBLE_C                     } )
#define CHAR_11    ( (char  [2]) { CHAR_A  , CHAR_A             } )
#define CHAR_12    ( (char  [2]) { CHAR_A  , CHAR_B             } )
#define CHAR_13    ( (char  [2]) { CHAR_A  , CHAR_C             } )
#define CHAR_21    ( (char  [2]) { CHAR_B  , CHAR_A             } )
#define CHAR_22    ( (char  [2]) { CHAR_B  , CHAR_B             } )
#define CHAR_23    ( (char  [2]) { CHAR_B  , CHAR_C             } )
#define CHAR_31    ( (char  [2]) { CHAR_C  , CHAR_A             } )
#define CHAR_32    ( (char  [2]) { CHAR_C  , CHAR_B             } )
#define CHAR_33    ( (char  [2]) { CHAR_C  , CHAR_C             } )
#define SHORT_11   ( (short [2]) { SHORT_A , SHORT_A            } )
#define SHORT_12   ( (short [2]) { SHORT_A , SHORT_B            } )
#define SHORT_13   ( (short [2]) { SHORT_A , SHORT_C            } )
#define SHORT_21   ( (short [2]) { SHORT_B , SHORT_A            } )
#define SHORT_22   ( (short [2]) { SHORT_B , SHORT_B            } )
#define SHORT_23   ( (short [2]) { SHORT_B , SHORT_C            } )
#define SHORT_31   ( (short [2]) { SHORT_C , SHORT_A            } )
#define SHORT_32   ( (short [2]) { SHORT_C , SHORT_B            } )
#define SHORT_33   ( (short [2]) { SHORT_C , SHORT_C            } )
#define INT_11     ( (int   [2]) { INT_A   , INT_A              } )
#define INT_12     ( (int   [2]) { INT_A   , INT_B              } )
#define INT_13     ( (int   [2]) { INT_A   , INT_C              } )
#define INT_21     ( (int   [2]) { INT_B   , INT_A              } )
#define INT_22     ( (int   [2]) { INT_B   , INT_B              } )
#define INT_23     ( (int   [2]) { INT_B   , INT_C              } )
#define INT_31     ( (int   [2]) { INT_C   , INT_A              } )
#define INT_32     ( (int   [2]) { INT_C   , INT_B              } )
#define INT_33     ( (int   [2]) { INT_C   , INT_C              } )
#define LONG_11    ( (long  [2]) { LONG_A  , LONG_A             } )
#define LONG_12    ( (long  [2]) { LONG_A  , LONG_B             } )
#define LONG_13    ( (long  [2]) { LONG_A  , LONG_C             } )
#define LONG_21    ( (long  [2]) { LONG_B  , LONG_A             } )
#define LONG_22    ( (long  [2]) { LONG_B  , LONG_B             } )
#define LONG_23    ( (long  [2]) { LONG_B  , LONG_C             } )
#define LONG_31    ( (long  [2]) { LONG_C  , LONG_A             } )
#define LONG_32    ( (long  [2]) { LONG_C  , LONG_B             } )
#define LONG_33    ( (long  [2]) { LONG_C  , LONG_C             } )
#define FLOAT_11   ( (float [2]) { FLOAT_A , FLOAT_A            } )
#define FLOAT_12   ( (float [2]) { FLOAT_A , FLOAT_B            } )
#define FLOAT_13   ( (float [2]) { FLOAT_A , FLOAT_C            } )
#define FLOAT_21   ( (float [2]) { FLOAT_B , FLOAT_A            } )
#define FLOAT_22   ( (float [2]) { FLOAT_B , FLOAT_B            } )
#define FLOAT_23   ( (float [2]) { FLOAT_B , FLOAT_C            } )
#define FLOAT_31   ( (float [2]) { FLOAT_C , FLOAT_A            } )
#define FLOAT_32   ( (float [2]) { FLOAT_C , FLOAT_B            } )
#define FLOAT_33   ( (float [2]) { FLOAT_C , FLOAT_C            } )
#define DOUBLE_11  ( (double[2]) { DOUBLE_A, DOUBLE_A           } )
#define DOUBLE_12  ( (double[2]) { DOUBLE_A, DOUBLE_B           } )
#define DOUBLE_13  ( (double[2]) { DOUBLE_A, DOUBLE_C           } )
#define DOUBLE_21  ( (double[2]) { DOUBLE_B, DOUBLE_A           } )
#define DOUBLE_22  ( (double[2]) { DOUBLE_B, DOUBLE_B           } )
#define DOUBLE_23  ( (double[2]) { DOUBLE_B, DOUBLE_C           } )
#define DOUBLE_31  ( (double[2]) { DOUBLE_C, DOUBLE_A           } )
#define DOUBLE_32  ( (double[2]) { DOUBLE_C, DOUBLE_B           } )
#define DOUBLE_33  ( (double[2]) { DOUBLE_C, DOUBLE_C           } )
#define CHAR_111   ( (char  [3]) { CHAR_A  , CHAR_A  , CHAR_A   } )
#define CHAR_112   ( (char  [3]) { CHAR_A  , CHAR_A  , CHAR_B   } )
#define CHAR_113   ( (char  [3]) { CHAR_A  , CHAR_A  , CHAR_C   } )
#define CHAR_121   ( (char  [3]) { CHAR_A  , CHAR_B  , CHAR_A   } )
#define CHAR_122   ( (char  [3]) { CHAR_A  , CHAR_B  , CHAR_B   } )
#define CHAR_123   ( (char  [3]) { CHAR_A  , CHAR_B  , CHAR_C   } )
#define CHAR_131   ( (char  [3]) { CHAR_A  , CHAR_C  , CHAR_A   } )
#define CHAR_132   ( (char  [3]) { CHAR_A  , CHAR_C  , CHAR_B   } )
#define CHAR_133   ( (char  [3]) { CHAR_A  , CHAR_C  , CHAR_C   } )
#define CHAR_211   ( (char  [3]) { CHAR_B  , CHAR_A  , CHAR_A   } )
#define CHAR_212   ( (char  [3]) { CHAR_B  , CHAR_A  , CHAR_B   } )
#define CHAR_213   ( (char  [3]) { CHAR_B  , CHAR_A  , CHAR_C   } )
#define CHAR_221   ( (char  [3]) { CHAR_B  , CHAR_B  , CHAR_A   } )
#define CHAR_222   ( (char  [3]) { CHAR_B  , CHAR_B  , CHAR_B   } )
#define CHAR_223   ( (char  [3]) { CHAR_B  , CHAR_B  , CHAR_C   } )
#define CHAR_231   ( (char  [3]) { CHAR_B  , CHAR_C  , CHAR_A   } )
#define CHAR_232   ( (char  [3]) { CHAR_B  , CHAR_C  , CHAR_B   } )
#define CHAR_233   ( (char  [3]) { CHAR_B  , CHAR_C  , CHAR_C   } )
#define CHAR_311   ( (char  [3]) { CHAR_C  , CHAR_A  , CHAR_A   } )
#define CHAR_312   ( (char  [3]) { CHAR_C  , CHAR_A  , CHAR_B   } )
#define CHAR_313   ( (char  [3]) { CHAR_C  , CHAR_A  , CHAR_C   } )
#define CHAR_321   ( (char  [3]) { CHAR_C  , CHAR_B  , CHAR_A   } )
#define CHAR_322   ( (char  [3]) { CHAR_C  , CHAR_B  , CHAR_B   } )
#define CHAR_323   ( (char  [3]) { CHAR_C  , CHAR_B  , CHAR_C   } )
#define CHAR_331   ( (char  [3]) { CHAR_C  , CHAR_C  , CHAR_A   } )
#define CHAR_332   ( (char  [3]) { CHAR_C  , CHAR_C  , CHAR_B   } )
#define CHAR_333   ( (char  [3]) { CHAR_C  , CHAR_C  , CHAR_C   } )
#define SHORT_111  ( (short [3]) { SHORT_A , SHORT_A , SHORT_A  } )
#define SHORT_112  ( (short [3]) { SHORT_A , SHORT_A , SHORT_B  } )
#define SHORT_113  ( (short [3]) { SHORT_A , SHORT_A , SHORT_C  } )
#define SHORT_121  ( (short [3]) { SHORT_A , SHORT_B , SHORT_A  } )
#define SHORT_122  ( (short [3]) { SHORT_A , SHORT_B , SHORT_B  } )
#define SHORT_123  ( (short [3]) { SHORT_A , SHORT_B , SHORT_C  } )
#define SHORT_131  ( (short [3]) { SHORT_A , SHORT_C , SHORT_A  } )
#define SHORT_132  ( (short [3]) { SHORT_A , SHORT_C , SHORT_B  } )
#define SHORT_133  ( (short [3]) { SHORT_A , SHORT_C , SHORT_C  } )
#define SHORT_211  ( (short [3]) { SHORT_B , SHORT_A , SHORT_A  } )
#define SHORT_212  ( (short [3]) { SHORT_B , SHORT_A , SHORT_B  } )
#define SHORT_213  ( (short [3]) { SHORT_B , SHORT_A , SHORT_C  } )
#define SHORT_221  ( (short [3]) { SHORT_B , SHORT_B , SHORT_A  } )
#define SHORT_222  ( (short [3]) { SHORT_B , SHORT_B , SHORT_B  } )
#define SHORT_223  ( (short [3]) { SHORT_B , SHORT_B , SHORT_C  } )
#define SHORT_231  ( (short [3]) { SHORT_B , SHORT_C , SHORT_A  } )
#define SHORT_232  ( (short [3]) { SHORT_B , SHORT_C , SHORT_B  } )
#define SHORT_233  ( (short [3]) { SHORT_B , SHORT_C , SHORT_C  } )
#define SHORT_311  ( (short [3]) { SHORT_C , SHORT_A , SHORT_A  } )
#define SHORT_312  ( (short [3]) { SHORT_C , SHORT_A , SHORT_B  } )
#define SHORT_313  ( (short [3]) { SHORT_C , SHORT_A , SHORT_C  } )
#define SHORT_321  ( (short [3]) { SHORT_C , SHORT_B , SHORT_A  } )
#define SHORT_322  ( (short [3]) { SHORT_C , SHORT_B , SHORT_B  } )
#define SHORT_323  ( (short [3]) { SHORT_C , SHORT_B , SHORT_C  } )
#define SHORT_331  ( (short [3]) { SHORT_C , SHORT_C , SHORT_A  } )
#define SHORT_332  ( (short [3]) { SHORT_C , SHORT_C , SHORT_B  } )
#define SHORT_333  ( (short [3]) { SHORT_C , SHORT_C , SHORT_C  } )
#define INT_111    ( (int   [3]) { INT_A   , INT_A   , INT_A    } )
#define INT_112    ( (int   [3]) { INT_A   , INT_A   , INT_B    } )
#define INT_113    ( (int   [3]) { INT_A   , INT_A   , INT_C    } )
#define INT_121    ( (int   [3]) { INT_A   , INT_B   , INT_A    } )
#define INT_122    ( (int   [3]) { INT_A   , INT_B   , INT_B    } )
#define INT_123    ( (int   [3]) { INT_A   , INT_B   , INT_C    } )
#define INT_131    ( (int   [3]) { INT_A   , INT_C   , INT_A    } )
#define INT_132    ( (int   [3]) { INT_A   , INT_C   , INT_B    } )
#define INT_133    ( (int   [3]) { INT_A   , INT_C   , INT_C    } )
#define INT_211    ( (int   [3]) { INT_B   , INT_A   , INT_A    } )
#define INT_212    ( (int   [3]) { INT_B   , INT_A   , INT_B    } )
#define INT_213    ( (int   [3]) { INT_B   , INT_A   , INT_C    } )
#define INT_221    ( (int   [3]) { INT_B   , INT_B   , INT_A    } )
#define INT_222    ( (int   [3]) { INT_B   , INT_B   , INT_B    } )
#define INT_223    ( (int   [3]) { INT_B   , INT_B   , INT_C    } )
#define INT_231    ( (int   [3]) { INT_B   , INT_C   , INT_A    } )
#define INT_232    ( (int   [3]) { INT_B   , INT_C   , INT_B    } )
#define INT_233    ( (int   [3]) { INT_B   , INT_C   , INT_C    } )
#define INT_311    ( (int   [3]) { INT_C   , INT_A   , INT_A    } )
#define INT_312    ( (int   [3]) { INT_C   , INT_A   , INT_B    } )
#define INT_313    ( (int   [3]) { INT_C   , INT_A   , INT_C    } )
#define INT_321    ( (int   [3]) { INT_C   , INT_B   , INT_A    } )
#define INT_322    ( (int   [3]) { INT_C   , INT_B   , INT_B    } )
#define INT_323    ( (int   [3]) { INT_C   , INT_B   , INT_C    } )
#define INT_331    ( (int   [3]) { INT_C   , INT_C   , INT_A    } )
#define INT_332    ( (int   [3]) { INT_C   , INT_C   , INT_B    } )
#define INT_333    ( (int   [3]) { INT_C   , INT_C   , INT_C    } )
#define LONG_111   ( (long  [3]) { LONG_A  , LONG_A  , LONG_A   } )
#define LONG_112   ( (long  [3]) { LONG_A  , LONG_A  , LONG_B   } )
#define LONG_113   ( (long  [3]) { LONG_A  , LONG_A  , LONG_C   } )
#define LONG_121   ( (long  [3]) { LONG_A  , LONG_B  , LONG_A   } )
#define LONG_122   ( (long  [3]) { LONG_A  , LONG_B  , LONG_B   } )
#define LONG_123   ( (long  [3]) { LONG_A  , LONG_B  , LONG_C   } )
#define LONG_131   ( (long  [3]) { LONG_A  , LONG_C  , LONG_A   } )
#define LONG_132   ( (long  [3]) { LONG_A  , LONG_C  , LONG_B   } )
#define LONG_133   ( (long  [3]) { LONG_A  , LONG_C  , LONG_C   } )
#define LONG_211   ( (long  [3]) { LONG_B  , LONG_A  , LONG_A   } )
#define LONG_212   ( (long  [3]) { LONG_B  , LONG_A  , LONG_B   } )
#define LONG_213   ( (long  [3]) { LONG_B  , LONG_A  , LONG_C   } )
#define LONG_221   ( (long  [3]) { LONG_B  , LONG_B  , LONG_A   } )
#define LONG_222   ( (long  [3]) { LONG_B  , LONG_B  , LONG_B   } )
#define LONG_223   ( (long  [3]) { LONG_B  , LONG_B  , LONG_C   } )
#define LONG_231   ( (long  [3]) { LONG_B  , LONG_C  , LONG_A   } )
#define LONG_232   ( (long  [3]) { LONG_B  , LONG_C  , LONG_B   } )
#define LONG_233   ( (long  [3]) { LONG_B  , LONG_C  , LONG_C   } )
#define LONG_311   ( (long  [3]) { LONG_C  , LONG_A  , LONG_A   } )
#define LONG_312   ( (long  [3]) { LONG_C  , LONG_A  , LONG_B   } )
#define LONG_313   ( (long  [3]) { LONG_C  , LONG_A  , LONG_C   } )
#define LONG_321   ( (long  [3]) { LONG_C  , LONG_B  , LONG_A   } )
#define LONG_322   ( (long  [3]) { LONG_C  , LONG_B  , LONG_B   } )
#define LONG_323   ( (long  [3]) { LONG_C  , LONG_B  , LONG_C   } )
#define LONG_331   ( (long  [3]) { LONG_C  , LONG_C  , LONG_A   } )
#define LONG_332   ( (long  [3]) { LONG_C  , LONG_C  , LONG_B   } )
#define LONG_333   ( (long  [3]) { LONG_C  , LONG_C  , LONG_C   } )
#define FLOAT_111  ( (float [3]) { FLOAT_A , FLOAT_A , FLOAT_A  } )
#define FLOAT_112  ( (float [3]) { FLOAT_A , FLOAT_A , FLOAT_B  } )
#define FLOAT_113  ( (float [3]) { FLOAT_A , FLOAT_A , FLOAT_C  } )
#define FLOAT_121  ( (float [3]) { FLOAT_A , FLOAT_B , FLOAT_A  } )
#define FLOAT_122  ( (float [3]) { FLOAT_A , FLOAT_B , FLOAT_B  } )
#define FLOAT_123  ( (float [3]) { FLOAT_A , FLOAT_B , FLOAT_C  } )
#define FLOAT_131  ( (float [3]) { FLOAT_A , FLOAT_C , FLOAT_A  } )
#define FLOAT_132  ( (float [3]) { FLOAT_A , FLOAT_C , FLOAT_B  } )
#define FLOAT_133  ( (float [3]) { FLOAT_A , FLOAT_C , FLOAT_C  } )
#define FLOAT_211  ( (float [3]) { FLOAT_B , FLOAT_A , FLOAT_A  } )
#define FLOAT_212  ( (float [3]) { FLOAT_B , FLOAT_A , FLOAT_B  } )
#define FLOAT_213  ( (float [3]) { FLOAT_B , FLOAT_A , FLOAT_C  } )
#define FLOAT_221  ( (float [3]) { FLOAT_B , FLOAT_B , FLOAT_A  } )
#define FLOAT_222  ( (float [3]) { FLOAT_B , FLOAT_B , FLOAT_B  } )
#define FLOAT_223  ( (float [3]) { FLOAT_B , FLOAT_B , FLOAT_C  } )
#define FLOAT_231  ( (float [3]) { FLOAT_B , FLOAT_C , FLOAT_A  } )
#define FLOAT_232  ( (float [3]) { FLOAT_B , FLOAT_C , FLOAT_B  } )
#define FLOAT_233  ( (float [3]) { FLOAT_B , FLOAT_C , FLOAT_C  } )
#define FLOAT_311  ( (float [3]) { FLOAT_C , FLOAT_A , FLOAT_A  } )
#define FLOAT_312  ( (float [3]) { FLOAT_C , FLOAT_A , FLOAT_B  } )
#define FLOAT_313  ( (float [3]) { FLOAT_C , FLOAT_A , FLOAT_C  } )
#define FLOAT_321  ( (float [3]) { FLOAT_C , FLOAT_B , FLOAT_A  } )
#define FLOAT_322  ( (float [3]) { FLOAT_C , FLOAT_B , FLOAT_B  } )
#define FLOAT_323  ( (float [3]) { FLOAT_C , FLOAT_B , FLOAT_C  } )
#define FLOAT_331  ( (float [3]) { FLOAT_C , FLOAT_C , FLOAT_A  } )
#define FLOAT_332  ( (float [3]) { FLOAT_C , FLOAT_C , FLOAT_B  } )
#define FLOAT_333  ( (float [3]) { FLOAT_C , FLOAT_C , FLOAT_C  } )
#define DOUBLE_111 ( (double[3]) { DOUBLE_A, DOUBLE_A, DOUBLE_A } )
#define DOUBLE_112 ( (double[3]) { DOUBLE_A, DOUBLE_A, DOUBLE_B } )
#define DOUBLE_113 ( (double[3]) { DOUBLE_A, DOUBLE_A, DOUBLE_C } )
#define DOUBLE_121 ( (double[3]) { DOUBLE_A, DOUBLE_B, DOUBLE_A } )
#define DOUBLE_122 ( (double[3]) { DOUBLE_A, DOUBLE_B, DOUBLE_B } )
#define DOUBLE_123 ( (double[3]) { DOUBLE_A, DOUBLE_B, DOUBLE_C } )
#define DOUBLE_131 ( (double[3]) { DOUBLE_A, DOUBLE_C, DOUBLE_A } )
#define DOUBLE_132 ( (double[3]) { DOUBLE_A, DOUBLE_C, DOUBLE_B } )
#define DOUBLE_133 ( (double[3]) { DOUBLE_A, DOUBLE_C, DOUBLE_C } )
#define DOUBLE_211 ( (double[3]) { DOUBLE_B, DOUBLE_A, DOUBLE_A } )
#define DOUBLE_212 ( (double[3]) { DOUBLE_B, DOUBLE_A, DOUBLE_B } )
#define DOUBLE_213 ( (double[3]) { DOUBLE_B, DOUBLE_A, DOUBLE_C } )
#define DOUBLE_221 ( (double[3]) { DOUBLE_B, DOUBLE_B, DOUBLE_A } )
#define DOUBLE_222 ( (double[3]) { DOUBLE_B, DOUBLE_B, DOUBLE_B } )
#define DOUBLE_223 ( (double[3]) { DOUBLE_B, DOUBLE_B, DOUBLE_C } )
#define DOUBLE_231 ( (double[3]) { DOUBLE_B, DOUBLE_C, DOUBLE_A } )
#define DOUBLE_232 ( (double[3]) { DOUBLE_B, DOUBLE_C, DOUBLE_B } )
#define DOUBLE_233 ( (double[3]) { DOUBLE_B, DOUBLE_C, DOUBLE_C } )
#define DOUBLE_311 ( (double[3]) { DOUBLE_C, DOUBLE_A, DOUBLE_A } )
#define DOUBLE_312 ( (double[3]) { DOUBLE_C, DOUBLE_A, DOUBLE_B } )
#define DOUBLE_313 ( (double[3]) { DOUBLE_C, DOUBLE_A, DOUBLE_C } )
#define DOUBLE_321 ( (double[3]) { DOUBLE_C, DOUBLE_B, DOUBLE_A } )
#define DOUBLE_322 ( (double[3]) { DOUBLE_C, DOUBLE_B, DOUBLE_B } )
#define DOUBLE_323 ( (double[3]) { DOUBLE_C, DOUBLE_B, DOUBLE_C } )
#define DOUBLE_331 ( (double[3]) { DOUBLE_C, DOUBLE_C, DOUBLE_A } )
#define DOUBLE_332 ( (double[3]) { DOUBLE_C, DOUBLE_C, DOUBLE_B } )
#define DOUBLE_333 ( (double[3]) { DOUBLE_C, DOUBLE_C, DOUBLE_C } )

// function declarations
/// scenario constructors
fn_scenario_constructor construct_stream;

/// test cases
fn_test_case test_pack_unpack_1i8;
fn_test_case test_pack_unpack_2i8;
fn_test_case test_pack_unpack_3i8;
fn_test_case test_pack_unpack_1i16;
fn_test_case test_pack_unpack_2i16;
fn_test_case test_pack_unpack_3i16;
fn_test_case test_pack_unpack_1i32;
fn_test_case test_pack_unpack_2i32;
fn_test_case test_pack_unpack_3i32;
fn_test_case test_pack_unpack_1i64;
fn_test_case test_pack_unpack_2i64;
fn_test_case test_pack_unpack_3i64;
fn_test_case test_pack_unpack_1f32;
fn_test_case test_pack_unpack_2f32;
fn_test_case test_pack_unpack_3f32;
fn_test_case test_pack_unpack_1f64;
fn_test_case test_pack_unpack_2f64;
fn_test_case test_pack_unpack_3f64;

/// allocators
fn_allocator destruct_stream;

// test
/// cases
test_case _i8[] =
{
    TEST_CASE ("{ 1 }"      , test_pack_unpack_1i8, CHAR_1, TEST_RESULT_ONE),
    TEST_CASE ("{ 2 }"      , test_pack_unpack_1i8, CHAR_2, TEST_RESULT_ONE),
    TEST_CASE ("{ 3 }"      , test_pack_unpack_1i8, CHAR_3, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1 }"   , test_pack_unpack_2i8, CHAR_11, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2 }"   , test_pack_unpack_2i8, CHAR_12, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3 }"   , test_pack_unpack_2i8, CHAR_13, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1 }"   , test_pack_unpack_2i8, CHAR_21, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2 }"   , test_pack_unpack_2i8, CHAR_22, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3 }"   , test_pack_unpack_2i8, CHAR_23, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1 }"   , test_pack_unpack_2i8, CHAR_31, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2 }"   , test_pack_unpack_2i8, CHAR_32, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3 }"   , test_pack_unpack_2i8, CHAR_33, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 1 }", test_pack_unpack_3i8, CHAR_111, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 2 }", test_pack_unpack_3i8, CHAR_112, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 3 }", test_pack_unpack_3i8, CHAR_113, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 1 }", test_pack_unpack_3i8, CHAR_121, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 2 }", test_pack_unpack_3i8, CHAR_122, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 3 }", test_pack_unpack_3i8, CHAR_123, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 1 }", test_pack_unpack_3i8, CHAR_131, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 2 }", test_pack_unpack_3i8, CHAR_132, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 3 }", test_pack_unpack_3i8, CHAR_133, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 1 }", test_pack_unpack_3i8, CHAR_211, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 2 }", test_pack_unpack_3i8, CHAR_212, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 3 }", test_pack_unpack_3i8, CHAR_213, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 1 }", test_pack_unpack_3i8, CHAR_221, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 2 }", test_pack_unpack_3i8, CHAR_222, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 3 }", test_pack_unpack_3i8, CHAR_223, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 1 }", test_pack_unpack_3i8, CHAR_231, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 2 }", test_pack_unpack_3i8, CHAR_232, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 3 }", test_pack_unpack_3i8, CHAR_233, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 1 }", test_pack_unpack_3i8, CHAR_311, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 2 }", test_pack_unpack_3i8, CHAR_312, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 3 }", test_pack_unpack_3i8, CHAR_313, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 1 }", test_pack_unpack_3i8, CHAR_321, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 2 }", test_pack_unpack_3i8, CHAR_322, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 3 }", test_pack_unpack_3i8, CHAR_323, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 1 }", test_pack_unpack_3i8, CHAR_331, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 2 }", test_pack_unpack_3i8, CHAR_332, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 3 }", test_pack_unpack_3i8, CHAR_333, TEST_RESULT_ONE),
};

test_case _i16[] =
{
    TEST_CASE ("{ 1 }"      , test_pack_unpack_1i16, SHORT_1, TEST_RESULT_ONE),
    TEST_CASE ("{ 2 }"      , test_pack_unpack_1i16, SHORT_2, TEST_RESULT_ONE),
    TEST_CASE ("{ 3 }"      , test_pack_unpack_1i16, SHORT_3, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1 }"   , test_pack_unpack_2i16, SHORT_11, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2 }"   , test_pack_unpack_2i16, SHORT_12, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3 }"   , test_pack_unpack_2i16, SHORT_13, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1 }"   , test_pack_unpack_2i16, SHORT_21, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2 }"   , test_pack_unpack_2i16, SHORT_22, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3 }"   , test_pack_unpack_2i16, SHORT_23, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1 }"   , test_pack_unpack_2i16, SHORT_31, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2 }"   , test_pack_unpack_2i16, SHORT_32, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3 }"   , test_pack_unpack_2i16, SHORT_33, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 1 }", test_pack_unpack_3i16, SHORT_111, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 2 }", test_pack_unpack_3i16, SHORT_112, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 3 }", test_pack_unpack_3i16, SHORT_113, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 1 }", test_pack_unpack_3i16, SHORT_121, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 2 }", test_pack_unpack_3i16, SHORT_122, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 3 }", test_pack_unpack_3i16, SHORT_123, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 1 }", test_pack_unpack_3i16, SHORT_131, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 2 }", test_pack_unpack_3i16, SHORT_132, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 3 }", test_pack_unpack_3i16, SHORT_133, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 1 }", test_pack_unpack_3i16, SHORT_211, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 2 }", test_pack_unpack_3i16, SHORT_212, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 3 }", test_pack_unpack_3i16, SHORT_213, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 1 }", test_pack_unpack_3i16, SHORT_221, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 2 }", test_pack_unpack_3i16, SHORT_222, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 3 }", test_pack_unpack_3i16, SHORT_223, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 1 }", test_pack_unpack_3i16, SHORT_231, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 2 }", test_pack_unpack_3i16, SHORT_232, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 3 }", test_pack_unpack_3i16, SHORT_233, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 1 }", test_pack_unpack_3i16, SHORT_311, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 2 }", test_pack_unpack_3i16, SHORT_312, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 3 }", test_pack_unpack_3i16, SHORT_313, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 1 }", test_pack_unpack_3i16, SHORT_321, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 2 }", test_pack_unpack_3i16, SHORT_322, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 3 }", test_pack_unpack_3i16, SHORT_323, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 1 }", test_pack_unpack_3i16, SHORT_331, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 2 }", test_pack_unpack_3i16, SHORT_332, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 3 }", test_pack_unpack_3i16, SHORT_333, TEST_RESULT_ONE),
};

test_case _i32[] =
{
    TEST_CASE ("{ 1 }"      , test_pack_unpack_1i32, INT_1, TEST_RESULT_ONE),
    TEST_CASE ("{ 2 }"      , test_pack_unpack_1i32, INT_2, TEST_RESULT_ONE),
    TEST_CASE ("{ 3 }"      , test_pack_unpack_1i32, INT_3, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1 }"   , test_pack_unpack_2i32, INT_11, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2 }"   , test_pack_unpack_2i32, INT_12, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3 }"   , test_pack_unpack_2i32, INT_13, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1 }"   , test_pack_unpack_2i32, INT_21, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2 }"   , test_pack_unpack_2i32, INT_22, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3 }"   , test_pack_unpack_2i32, INT_23, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1 }"   , test_pack_unpack_2i32, INT_31, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2 }"   , test_pack_unpack_2i32, INT_32, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3 }"   , test_pack_unpack_2i32, INT_33, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 1 }", test_pack_unpack_3i32, INT_111, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 2 }", test_pack_unpack_3i32, INT_112, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 3 }", test_pack_unpack_3i32, INT_113, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 1 }", test_pack_unpack_3i32, INT_121, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 2 }", test_pack_unpack_3i32, INT_122, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 3 }", test_pack_unpack_3i32, INT_123, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 1 }", test_pack_unpack_3i32, INT_131, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 2 }", test_pack_unpack_3i32, INT_132, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 3 }", test_pack_unpack_3i32, INT_133, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 1 }", test_pack_unpack_3i32, INT_211, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 2 }", test_pack_unpack_3i32, INT_212, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 3 }", test_pack_unpack_3i32, INT_213, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 1 }", test_pack_unpack_3i32, INT_221, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 2 }", test_pack_unpack_3i32, INT_222, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 3 }", test_pack_unpack_3i32, INT_223, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 1 }", test_pack_unpack_3i32, INT_231, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 2 }", test_pack_unpack_3i32, INT_232, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 3 }", test_pack_unpack_3i32, INT_233, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 1 }", test_pack_unpack_3i32, INT_311, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 2 }", test_pack_unpack_3i32, INT_312, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 3 }", test_pack_unpack_3i32, INT_313, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 1 }", test_pack_unpack_3i32, INT_321, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 2 }", test_pack_unpack_3i32, INT_322, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 3 }", test_pack_unpack_3i32, INT_323, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 1 }", test_pack_unpack_3i32, INT_331, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 2 }", test_pack_unpack_3i32, INT_332, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 3 }", test_pack_unpack_3i32, INT_333, TEST_RESULT_ONE),
};

test_case _i64[] =
{
    TEST_CASE ("{ 1 }"      , test_pack_unpack_1i64, LONG_1, TEST_RESULT_ONE),
    TEST_CASE ("{ 2 }"      , test_pack_unpack_1i64, LONG_2, TEST_RESULT_ONE),
    TEST_CASE ("{ 3 }"      , test_pack_unpack_1i64, LONG_3, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1 }"   , test_pack_unpack_2i64, LONG_11, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2 }"   , test_pack_unpack_2i64, LONG_12, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3 }"   , test_pack_unpack_2i64, LONG_13, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1 }"   , test_pack_unpack_2i64, LONG_21, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2 }"   , test_pack_unpack_2i64, LONG_22, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3 }"   , test_pack_unpack_2i64, LONG_23, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1 }"   , test_pack_unpack_2i64, LONG_31, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2 }"   , test_pack_unpack_2i64, LONG_32, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3 }"   , test_pack_unpack_2i64, LONG_33, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 1 }", test_pack_unpack_3i64, LONG_111, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 2 }", test_pack_unpack_3i64, LONG_112, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 3 }", test_pack_unpack_3i64, LONG_113, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 1 }", test_pack_unpack_3i64, LONG_121, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 2 }", test_pack_unpack_3i64, LONG_122, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 3 }", test_pack_unpack_3i64, LONG_123, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 1 }", test_pack_unpack_3i64, LONG_131, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 2 }", test_pack_unpack_3i64, LONG_132, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 3 }", test_pack_unpack_3i64, LONG_133, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 1 }", test_pack_unpack_3i64, LONG_211, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 2 }", test_pack_unpack_3i64, LONG_212, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 3 }", test_pack_unpack_3i64, LONG_213, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 1 }", test_pack_unpack_3i64, LONG_221, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 2 }", test_pack_unpack_3i64, LONG_222, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 3 }", test_pack_unpack_3i64, LONG_223, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 1 }", test_pack_unpack_3i64, LONG_231, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 2 }", test_pack_unpack_3i64, LONG_232, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 3 }", test_pack_unpack_3i64, LONG_233, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 1 }", test_pack_unpack_3i64, LONG_311, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 2 }", test_pack_unpack_3i64, LONG_312, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 3 }", test_pack_unpack_3i64, LONG_313, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 1 }", test_pack_unpack_3i64, LONG_321, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 2 }", test_pack_unpack_3i64, LONG_322, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 3 }", test_pack_unpack_3i64, LONG_323, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 1 }", test_pack_unpack_3i64, LONG_331, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 2 }", test_pack_unpack_3i64, LONG_332, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 3 }", test_pack_unpack_3i64, LONG_333, TEST_RESULT_ONE),
};

test_case _f32[] =
{
    TEST_CASE ("{ 1 }"      , test_pack_unpack_1f32, FLOAT_1, TEST_RESULT_ONE),
    TEST_CASE ("{ 2 }"      , test_pack_unpack_1f32, FLOAT_2, TEST_RESULT_ONE),
    TEST_CASE ("{ 3 }"      , test_pack_unpack_1f32, FLOAT_3, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1 }"   , test_pack_unpack_2f32, FLOAT_11, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2 }"   , test_pack_unpack_2f32, FLOAT_12, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3 }"   , test_pack_unpack_2f32, FLOAT_13, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1 }"   , test_pack_unpack_2f32, FLOAT_21, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2 }"   , test_pack_unpack_2f32, FLOAT_22, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3 }"   , test_pack_unpack_2f32, FLOAT_23, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1 }"   , test_pack_unpack_2f32, FLOAT_31, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2 }"   , test_pack_unpack_2f32, FLOAT_32, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3 }"   , test_pack_unpack_2f32, FLOAT_33, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 1 }", test_pack_unpack_3f32, FLOAT_111, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 2 }", test_pack_unpack_3f32, FLOAT_112, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 3 }", test_pack_unpack_3f32, FLOAT_113, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 1 }", test_pack_unpack_3f32, FLOAT_121, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 2 }", test_pack_unpack_3f32, FLOAT_122, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 3 }", test_pack_unpack_3f32, FLOAT_123, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 1 }", test_pack_unpack_3f32, FLOAT_131, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 2 }", test_pack_unpack_3f32, FLOAT_132, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 3 }", test_pack_unpack_3f32, FLOAT_133, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 1 }", test_pack_unpack_3f32, FLOAT_211, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 2 }", test_pack_unpack_3f32, FLOAT_212, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 3 }", test_pack_unpack_3f32, FLOAT_213, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 1 }", test_pack_unpack_3f32, FLOAT_221, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 2 }", test_pack_unpack_3f32, FLOAT_222, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 3 }", test_pack_unpack_3f32, FLOAT_223, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 1 }", test_pack_unpack_3f32, FLOAT_231, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 2 }", test_pack_unpack_3f32, FLOAT_232, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 3 }", test_pack_unpack_3f32, FLOAT_233, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 1 }", test_pack_unpack_3f32, FLOAT_311, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 2 }", test_pack_unpack_3f32, FLOAT_312, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 3 }", test_pack_unpack_3f32, FLOAT_313, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 1 }", test_pack_unpack_3f32, FLOAT_321, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 2 }", test_pack_unpack_3f32, FLOAT_322, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 3 }", test_pack_unpack_3f32, FLOAT_323, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 1 }", test_pack_unpack_3f32, FLOAT_331, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 2 }", test_pack_unpack_3f32, FLOAT_332, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 3 }", test_pack_unpack_3f32, FLOAT_333, TEST_RESULT_ONE),
};

test_case _f64[] =
{
    TEST_CASE ("{ 1 }"      , test_pack_unpack_1f64, DOUBLE_1, TEST_RESULT_ONE),
    TEST_CASE ("{ 2 }"      , test_pack_unpack_1f64, DOUBLE_2, TEST_RESULT_ONE),
    TEST_CASE ("{ 3 }"      , test_pack_unpack_1f64, DOUBLE_3, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1 }"   , test_pack_unpack_2f64, DOUBLE_11, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2 }"   , test_pack_unpack_2f64, DOUBLE_12, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3 }"   , test_pack_unpack_2f64, DOUBLE_13, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1 }"   , test_pack_unpack_2f64, DOUBLE_21, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2 }"   , test_pack_unpack_2f64, DOUBLE_22, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3 }"   , test_pack_unpack_2f64, DOUBLE_23, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1 }"   , test_pack_unpack_2f64, DOUBLE_31, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2 }"   , test_pack_unpack_2f64, DOUBLE_32, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3 }"   , test_pack_unpack_2f64, DOUBLE_33, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 1 }", test_pack_unpack_3f64, DOUBLE_111, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 2 }", test_pack_unpack_3f64, DOUBLE_112, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 1, 3 }", test_pack_unpack_3f64, DOUBLE_113, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 1 }", test_pack_unpack_3f64, DOUBLE_121, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 2 }", test_pack_unpack_3f64, DOUBLE_122, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 2, 3 }", test_pack_unpack_3f64, DOUBLE_123, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 1 }", test_pack_unpack_3f64, DOUBLE_131, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 2 }", test_pack_unpack_3f64, DOUBLE_132, TEST_RESULT_ONE),
    TEST_CASE ("{ 1, 3, 3 }", test_pack_unpack_3f64, DOUBLE_133, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 1 }", test_pack_unpack_3f64, DOUBLE_211, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 2 }", test_pack_unpack_3f64, DOUBLE_212, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 1, 3 }", test_pack_unpack_3f64, DOUBLE_213, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 1 }", test_pack_unpack_3f64, DOUBLE_221, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 2 }", test_pack_unpack_3f64, DOUBLE_222, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 2, 3 }", test_pack_unpack_3f64, DOUBLE_223, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 1 }", test_pack_unpack_3f64, DOUBLE_231, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 2 }", test_pack_unpack_3f64, DOUBLE_232, TEST_RESULT_ONE),
    TEST_CASE ("{ 2, 3, 3 }", test_pack_unpack_3f64, DOUBLE_233, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 1 }", test_pack_unpack_3f64, DOUBLE_311, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 2 }", test_pack_unpack_3f64, DOUBLE_312, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 1, 3 }", test_pack_unpack_3f64, DOUBLE_313, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 1 }", test_pack_unpack_3f64, DOUBLE_321, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 2 }", test_pack_unpack_3f64, DOUBLE_322, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 2, 3 }", test_pack_unpack_3f64, DOUBLE_323, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 1 }", test_pack_unpack_3f64, DOUBLE_331, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 2 }", test_pack_unpack_3f64, DOUBLE_332, TEST_RESULT_ONE),
    TEST_CASE ("{ 3, 3, 3 }", test_pack_unpack_3f64, DOUBLE_333, TEST_RESULT_ONE),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("pack / unpack i8" , NULL, _i8 , construct_stream, destruct_stream),
    TEST_SCENARIO("pack / unpack i16", NULL, _i16, construct_stream, destruct_stream),
    TEST_SCENARIO("pack / unpack i32", NULL, _i32, construct_stream, destruct_stream),
    TEST_SCENARIO("pack / unpack i64", NULL, _i64, construct_stream, destruct_stream),
    TEST_SCENARIO("pack / unpack f32", NULL, _f32, construct_stream, destruct_stream),
    TEST_SCENARIO("pack / unpack f64", NULL, _f64, construct_stream, destruct_stream),
};

/// suites
test_suite _suite = TEST_SUITE("pack", _scenarios);

// entry point
int main ( int argc, const char *argv[] ) 
{

    // unused
    (void) argc;
    (void) argv;
     
    // run the tests
    test_suite_test(&_suite); 
    
    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int construct_stream ( void **pp_result )
{
    
    // done
    return stream_from_dynamic_buffer((stream**)pp_result);
}

void *test_pack_unpack_1i8 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    char   (*values)     [1] = (char (*)[1])(size_t)p_test_case->p_parameters;
    char     peeked      [1] = { 0 };
    char     unpacked    [1] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the character
        bytes_written += pack_pack(p_stream, "%i8", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the character
        stream_read(p_stream, &peeked[i], sizeof(char));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the character
        bytes_read += pack_unpack(p_stream, "%i8", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_2i8 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    char   (*values)     [2] = (char (*)[2])(size_t)p_test_case->p_parameters;
    char     peeked      [2] = { 0 };
    char     unpacked    [2] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the character
        bytes_written += pack_pack(p_stream, "%i8", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the character
        stream_read(p_stream, &peeked[i], sizeof(char));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the character
        bytes_read += pack_unpack(p_stream, "%i8", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_3i8 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    char   (*values)     [3] = (char (*)[3])(size_t)p_test_case->p_parameters;
    char     peeked      [3] = { 0 };
    char     unpacked    [3] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the character
        bytes_written += pack_pack(p_stream, "%i8", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the character
        stream_read(p_stream, &peeked[i], sizeof(char));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the character
        bytes_read += pack_unpack(p_stream, "%i8", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_1i16 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    short  (*values)     [1] = (short (*)[1])(size_t)p_test_case->p_parameters;
    short    peeked      [1] = { 0 };
    short    unpacked    [1] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the shortacter
        bytes_written += pack_pack(p_stream, "%i16", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the shortacter
        stream_read(p_stream, &peeked[i], sizeof(short));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the shortacter
        bytes_read += pack_unpack(p_stream, "%i16", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_2i16 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    short  (*values)     [2] = (short (*)[2])(size_t)p_test_case->p_parameters;
    short    peeked      [2] = { 0 };
    short    unpacked    [2] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the shortacter
        bytes_written += pack_pack(p_stream, "%i16", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the shortacter
        stream_read(p_stream, &peeked[i], sizeof(short));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the shortacter
        bytes_read += pack_unpack(p_stream, "%i16", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_3i16 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    short  (*values)     [3] = (short (*)[3])(size_t)p_test_case->p_parameters;
    short    peeked      [3] = { 0 };
    short    unpacked    [3] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the shortacter
        bytes_written += pack_pack(p_stream, "%i16", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the shortacter
        stream_read(p_stream, &peeked[i], sizeof(short));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the shortacter
        bytes_read += pack_unpack(p_stream, "%i16", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_1i32 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    int    (*values)     [1] = (int (*)[1])(size_t)p_test_case->p_parameters;
    int      peeked      [1] = { 0 };
    int      unpacked    [1] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the intacter
        bytes_written += pack_pack(p_stream, "%i32", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the intacter
        stream_read(p_stream, &peeked[i], sizeof(int));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the intacter
        bytes_read += pack_unpack(p_stream, "%i32", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_2i32 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    int    (*values)     [2] = (int (*)[2])(size_t)p_test_case->p_parameters;
    int      peeked      [2] = { 0 };
    int      unpacked    [2] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the intacter
        bytes_written += pack_pack(p_stream, "%i32", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the intacter
        stream_read(p_stream, &peeked[i], sizeof(int));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the intacter
        bytes_read += pack_unpack(p_stream, "%i32", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_3i32 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    int    (*values)     [3] = (int (*)[3])(size_t)p_test_case->p_parameters;
    int      peeked      [3] = { 0 };
    int      unpacked    [3] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the intacter
        bytes_written += pack_pack(p_stream, "%i32", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the intacter
        stream_read(p_stream, &peeked[i], sizeof(int));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the intacter
        bytes_read += pack_unpack(p_stream, "%i32", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_1i64 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    long   (*values)     [1] = (long (*)[1])(size_t)p_test_case->p_parameters;
    long     peeked      [1] = { 0 };
    long     unpacked    [1] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the longacter
        bytes_written += pack_pack(p_stream, "%i64", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the longacter
        stream_read(p_stream, &peeked[i], sizeof(long));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the longacter
        bytes_read += pack_unpack(p_stream, "%i64", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_2i64 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    long   (*values)     [2] = (long (*)[2])(size_t)p_test_case->p_parameters;
    long     peeked      [2] = { 0 };
    long     unpacked    [2] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the longacter
        bytes_written += pack_pack(p_stream, "%i64", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the longacter
        stream_read(p_stream, &peeked[i], sizeof(long));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the longacter
        bytes_read += pack_unpack(p_stream, "%i64", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_3i64 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    long   (*values)     [3] = (long (*)[3])(size_t)p_test_case->p_parameters;
    long     peeked      [3] = { 0 };
    long     unpacked    [3] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the longacter
        bytes_written += pack_pack(p_stream, "%i64", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the longacter
        stream_read(p_stream, &peeked[i], sizeof(long));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the longacter
        bytes_read += pack_unpack(p_stream, "%i64", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_1f32 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    float  (*values)     [1] = (float (*)[1])(size_t)p_test_case->p_parameters;
    float    peeked      [1] = { 0 };
    float    unpacked    [1] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the floatacter
        bytes_written += pack_pack(p_stream, "%f32", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the floatacter
        stream_read(p_stream, &peeked[i], sizeof(float));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the floatacter
        bytes_read += pack_unpack(p_stream, "%f32", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_2f32 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    float  (*values)     [2] = (float (*)[2])(size_t)p_test_case->p_parameters;
    float    peeked      [2] = { 0 };
    float    unpacked    [2] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the floatacter
        bytes_written += pack_pack(p_stream, "%f32", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the floatacter
        stream_read(p_stream, &peeked[i], sizeof(float));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the floatacter
        bytes_read += pack_unpack(p_stream, "%f32", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_3f32 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    float  (*values)     [3] = (float (*)[3])(size_t)p_test_case->p_parameters;
    float    peeked      [3] = { 0 };
    float    unpacked    [3] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the floatacter
        bytes_written += pack_pack(p_stream, "%f32", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the floatacter
        stream_read(p_stream, &peeked[i], sizeof(float));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the floatacter
        bytes_read += pack_unpack(p_stream, "%f32", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_1f64 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    double (*values)     [1] = (double (*)[1])(size_t)p_test_case->p_parameters;
    double   peeked      [1] = { 0 };
    double   unpacked    [1] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the doubleacter
        bytes_written += pack_pack(p_stream, "%f64", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the doubleacter
        stream_read(p_stream, &peeked[i], sizeof(double));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the doubleacter
        bytes_read += pack_unpack(p_stream, "%f64", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_2f64 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    double (*values)     [2] = (double (*)[2])(size_t)p_test_case->p_parameters;
    double   peeked      [2] = { 0 };
    double   unpacked    [2] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the doubleacter
        bytes_written += pack_pack(p_stream, "%f64", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the doubleacter
        stream_read(p_stream, &peeked[i], sizeof(double));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the doubleacter
        bytes_read += pack_unpack(p_stream, "%f64", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *test_pack_unpack_3f64 ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    stream  *p_stream        = (stream *)p_subject;
    double (*values)     [3] = (double (*)[3])(size_t)p_test_case->p_parameters;
    double   peeked      [3] = { 0 };
    double   unpacked    [3] = { 0 };
    size_t   bytes_written   = 0;
    size_t   bytes_read      = 0;
     
    // iterate through the values
    for (size_t i = 0; i < sizeof(*values)/sizeof(**values); i++)

        // pack the doubleacter
        bytes_written += pack_pack(p_stream, "%f64", *values[i]);

    // error check
    if ( sizeof(*values) != bytes_written ) return NULL;

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(peeked)/sizeof(*peeked); i++)
    {

        // peek the doubleacter
        stream_read(p_stream, &peeked[i], sizeof(double));

        // error check
        if ( *values[i] != peeked[i] ) return NULL;
    }
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // iterate through the values
    for (size_t i = 0; i < sizeof(unpacked)/sizeof(*unpacked); i++)
    {

        // unpack the doubleacter
        bytes_read += pack_unpack(p_stream, "%f64", &unpacked[i]);

        // error check
        if ( *values[i] != unpacked[i] ) return NULL;
    }

    // error check
    if ( bytes_written != bytes_read ) return NULL;

    // success
    return (void *)1;
}

void *destruct_stream ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    stream *p_stream = (stream *)p_pointer;

    // destroy the stream
    stream_destroy(&p_stream);

    // success
    return NULL;
}
