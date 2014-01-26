<?php

/******************************************************************************/
/*                                  RETTER                                    */
/******************************************************************************/
/*                                                                            */
/*    RTR0 (Retter engine) static class for PHP5                              */
/*    implemented and designed by Maciej Czyżewski                            */
/*    specification from http://maciejczyzewski.github.io/                    */
/*                                                                            */
/*    © Copyright 2014 Developer's Network. All rights reserved.              */
/*    This is licensed under the Lesser General Public License (LGPL)         */
/*    This library is free software; you can redistribute it and/or           */
/*    modify it under the terms of the GNU Lesser General Public              */
/*    License as published by the Free Software Foundation; either            */
/*    version 2.1 of the License, or (at your option) any later version.      */
/*                                                                            */
/*    This library is distributed in the hope that it will be useful,         */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        */
/*    Lesser General Public License for more details.                         */
/*                                                                            */
/*    You should have received a copy of the GNU Lesser General Public        */
/*    License along with this library; if not, write to the Free Software     */
/*    Foundation, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA       */
/*                                                                            */
/**** Version 0.0.1 ***********************************************************/
/*                                                                            */
/*    Syntax:                                                                 */
/*      string RTR0::hash( string message[, string format ])                  */
/*                                                                            */
/*    Description:                                                            */
/*      RTR0::hash() is a static function that must be called with `message`  */
/*      and optionally `format`. Possible values for `format` are:            */
/*      'bin'    binary string output                                         */
/*      'hex'    default; hexidecimal string output (lower case)              */
/*                                                                            */
/*      Failures return FALSE.                                                */
/*                                                                            */
/*    Usage:                                                                  */
/*      $hash = RTR0::hash('string to hash');                                 */
/*                                                                            */
/******************************************************************************/

class Retter {
    public function hash($string, $mode = 16)
    {

/********************************** STEP 1 ************************************/

        /* Unpack data to binary */
        $chunks = unpack('C*', $string);
        $length = count($chunks);

        /* Create a drawers with sample values */
        $A = 0x428a2f98;
        $B = 0xefbe4786;
        $C = 0xbef9a3f7;
        $D = 0xab1c5ed5;

        /* Addition of free bits */
        $chunks[0] = $A;    // First
        $chunks[]  = $B;    // length - 2
        $chunks[]  = $C;    // length - 1
        $chunks[]  = $D;    // Last

        /* Update length value */
        $length += 2;

/********************************** STEP 2 ************************************/

        /* Main loop */
        for( $i = 1; $i <= $length; $i++ )
        {
            /* Taking values ​​with neighbors */
            $a = $chunks[$i-1];
            $b = $chunks[$i];
            $c = $chunks[$i+1];

            /* Rotate values, insisting third value */
            $left = ($a >> $b % $c) | ($a << (sizeof($a) * 8 - $b % $c));
            $right = ($b >> $a % $c) | ($b << (sizeof($b) * 8 - $a % $c));

            /* Calculate sand for rotated values */
            $sand = abs(($left ^ $right + $a * $b) + $c);

            /* Create three distant situations */
            $round[0] = $sand % $mode;
            $round[1] = ($left + $right) % $mode;
            $round[2] = ($left - $right) % $mode;

            /* Addition of sand to the appropriate drawers */
            for( $j = 0; $j < 3; $j++ ){
                /* A: [0, 13, 2, 15] */
                if( ($round[$j] == 0)  || ($round[$j] == 13) 
                 || ($round[$j] == 2)  || ($round[$j] == 15) ){ $A += $sand >> 1; }
                /* B: [4, 9, 6, 11] */
                if( ($round[$j] == 4)  || ($round[$j] == 9)  
                 || ($round[$j] == 6)  || ($round[$j] == 11) ){ $B += $sand >> 1; }
                /* C: [8, 5, 10, 7] */
                if( ($round[$j] == 8)  || ($round[$j] == 5)
                 || ($round[$j] == 10) || ($round[$j] == 7)  ){ $C += $sand >> 1; }
                /* D: [12, 1, 14, 3] */
                if( ($round[$j] == 12) || ($round[$j] == 1)
                 || ($round[$j] == 14) || ($round[$j] == 3)  ){ $D += $sand >> 1; }
            }
        }

/********************************** STEP 3 ************************************/

        /* Calculate checksum for final values */
        $S = $A + $B + $C + $D;

        /* Creating four small hashes with checksum */
        $h[0] = sprintf('%08x', $A ^ $S);
        $h[1] = sprintf('%08x', $B ^ $S);
        $h[2] = sprintf('%08x', $C ^ $S);
        $h[3] = sprintf('%08x', $D ^ $S);

        /* Returns 124 bit, 32 hex final hash */
        return $h[0] . $h[1] . $h[2] . $h[3];
    }
}

/* Defining the current series of code */
class RTR0 extends Retter {}

