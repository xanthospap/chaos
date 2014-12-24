#ifndef _GNSSSTRUCTS_HPP
#define _GNSSSTRUCTS_HPP

/**
 * @file      gstructs.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      October, 2014
 *
 * @brief     Classes and functions for manipulating gnss data.
 *
 * @details   This file contains a list of classes, enums and functions for
 *            manipulating gnss data. It is written to assist the processing
 *            and analysis of Global Navigation Satellite Systems data.
 *            The library follows RINEX v3 specifications (@cite rnx302 ).
 *            -# <b>Satellite Numbers</b> (@cite rnx302 , section 3.5)<br>
 *              Starting with RINEX Version 2 the former two-digit satellite numbers 
 *              \c nn are preceded by a one-character system identifier \c s<br>
 *              Satellite System identifier (\c s) | Satellite System
 *              -----------------------------------|-----------------
 *                                               G | GPS<sup>1</sup>
 *                                               R | GLONASS
 *                                               S | SBAS payload
 *                                               E | Galileo
 *                                               C | BeiDou (BDS)
 *                                               J | QZSS
 *                                               M | Mixed<sup>2</sup>
 *                                               ? | Unknown<sup>3</sup>
 *                                               X | Any<sup>3</sup>
 *
 *              <tt>nn</tt> (integer) | Satellite System
 *              ----------------------|------------------------
 *                                PRN | GPS, Galileo, BeiDou
 *                        Slot number | Glonass
 *                            PRN-100 | SBAS, QZSS (L1 - SAIF)
 *                            PRN-192 | QZSS
 *
 *            -# <b>Observation Codes</b> (@cite rnx302 , section 5.1)<br>
 *             The new signal structures for GPS, Galileo and BDS make it possible 
 *             to generate code and phase observations based on one or a combination 
 *             of several channels: Two-channel signals are composed of I and Q 
 *             components, three-channel signals of A, B, and C components. Moreover 
 *             a wideband tracking of a combined E5a + E5b frequency tracking is 
 *             possible. In order to keep the observation codes short but still allow 
 *             for a detailed characterization of the actual signal generation the 
 *             length of the codes is increased from two (Version 1 and 2) to 
 *             three by adding a signal generation attribute:<br>
 *             The observation code \c tna consists of three parts:
@verbatim
+-----------------------------------------------------------------------------------------------+
| t : observation type | C = pseudorange | L = carrier phase | D = doppler | S = signal strength|
+----------------------+------------------------------------------------------------------------+
| n : band / frequency | 1, 2,...,8                                                             |
+----------------------+------------------------------------------------------------------------+
| a : attribute        | tracking mode or channel, e.g., I, Q, etc                              |
+----------------------+------------------------------------------------------------------------+
@endverbatim
 *            <b>Examples: </b> <br>
 *              - <b>L1C</b>: C/A code-derived L1 carrier phase (GPS, GLONASS) Carrier phase
 *                on E2-L1-E1 derived from C channel (Galileo)
 *              - <b>C2L</b>: L2C pseudorange derived from the L channel (GPS)
 *            For Galileo the band/frequency number \c n does not necessarily agree
 *            with the official frequency numbers: n = 7 for E5b, n = 8 for E5a+b.
 *            <br><b>Antispoofing (AS) of GPS:</b> True codeless GPS receivers 
 *            (squaring-type receivers) use the attribute \b N. Semi-codeless receivers 
 *            tracking the first frequency using C/A code and the second frequency
 *            using some codeless options use attribute \b D. \b Z-tracking under AS 
 *            or similar techniques to recover pseudorange and phase on the 'P-code' 
 *            band use attribute \b W. \b Y-code tracking receivers use attribute \b Y.
 *            As all observations affected by 'AS on' get now their own attribute 
 *            (codeless, semi-codeless, Z-tracking and similar) the Antispoofing 
 *            flag introduced into the observation data records of RINEX Version 
 *            2 has become obsolete.
 *            -# <b>Ionosphere delay as pseudo-observables</b> (@cite rnx302 , section 5.12)<br>
 *            RINEX files could also be used to make available additional information 
 *            linked to the actual observations. One such element is the ionosphere 
 *            delay having been determined or derived from an ionosphere model. We 
 *            add the ionosphere phase delay expressed in full cycles of the
 *            respective satellite system-dependent wavelength as pseudo-observable 
 *            to the list of the RINEX observables.
@verbatim
+---------------------------------------------------+
| t : observation type | I = Ionosphere phase delay |
+----------------------+----------------------------+
| n : band / frequency | 1, 2,...,8                 |
+----------------------+----------------------------+
| a : attribute        | blank                      |
+----------------------+----------------------------+
@endverbatim
 *            The ionosphere pseudo-observable has to be included into the list 
 *            of observables of the respective satellite system. Only one ionosphere
 *            observable per satellite is allowed.
 *            -# <b>Channel numbers as pseudo-observables</b> (@cite rnx302 , section 5.13)<br>
 *            For special applications it might be necessary to know the receiver 
 *            channel numbers having been assigned by the receiver to the individual 
 *            satellites. We may include this information as another pseudo-observable:
@verbatim
+----------------------------------------------------+
| t : observation type | X = Receiver channel number |
+----------------------+-----------------------------+
| n : band / frequency | 1                           |
+----------------------+-----------------------------+
| a : attribute        | blank                       |
+----------------------+-----------------------------+
@endverbatim
 *            Lowest channel number allowed is 1 (re-number channels beforehand, 
 *            if necessary). In case of a receiver using multiple channels for one
 *            satellite the channels could be packed with two digits each right-
 *            justified into the same data field, order corresponding to the order 
 *            of the observables concerned. Format F14.3 according to 
 *            <tt>(<5-nc>(2X),<nc>I2.2,'.000')</tt>, \c nc being the number of channels.
 *            \b Restriction: Not more than 5 channels and channel numbers <100.
 *            Examples:<br>
 *            - \b 0910.000 for channels 9 and 10
 *            - \b 010203.000 for channels 1, 2, and 3
 *
 * <table>
 *  <tr>
 *    <th rowspan="2">GNSS System</th>
 *    <th rowspan="2">Freq. Band / Frequency</th>
 *    <th rowspan="2">Channel or Code</th>
 *    <th colspan="4">Observation Codes</th>
 *  </tr>
 *  <tr>
 *    <th>PseudoRange</th>
 *    <th>Carrier Phase</th>
 *    <th>Doppler</th>
 *    <th>Signal Strength</th>
 *  </tr>
 *  <tr>
 *    <td rowspan="22">GPS</td>
 *    <td rowspan="9">L1/1575.42</td>
 *    <td>C/A</td>
 *    <td>C1C</td>
 *    <td>L1C</td>
 *    <td>D1C</td>
 *    <td>S1C</td>
 *  </tr>
 *  <tr>
 *    <td>L1C (D)</td>
 *    <td>C1S</td>
 *    <td>L1S</td>
 *    <td>D1S</td>
 *    <td>S1S</td>
 *  </tr>
 *  <tr>
 *    <td>L1C (P)</td>
 *    <td>C1L</td>
 *    <td>L1L</td>
 *    <td>D1L</td>
 *    <td>S1L</td>
 *  </tr>
 *  <tr>
 *    <td>L1C (D+P)</td>
 *    <td>C1X</td>
 *    <td>L1X</td>
 *    <td>D1X</td>
 *    <td>S1X</td>
 *  </tr>
 *  <tr>
 *    <td>P</td>
 *    <td>C1P</td>
 *    <td>L1P</td>
 *    <td>D1P</td>
 *    <td>S1P</td>
 *  </tr>
 *  <tr>
 *    <td>Z-tracking and similiar (AS on)</td>
 *    <td>C1W</td>
 *    <td>L1W</td>
 *    <td>D1W</td>
 *    <td>S1W</td>
 *  </tr>
 *  <tr>
 *    <td>Y</td>
 *    <td>C1Y</td>
 *    <td>L1Y</td>
 *    <td>D1Y</td>
 *    <td>S1Y</td>
 *  </tr>
 *  <tr>
 *    <td>M</td>
 *    <td>C1M</td>
 *    <td>L1M</td>
 *    <td>D1M</td>
 *    <td>S1M</td>
 *  </tr>
 *  <tr>
 *    <td>codeless</td>
 *    <td></td>
 *    <td>L1N</td>
 *    <td>D1N</td>
 *    <td>S1N</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="10">L2/1227.60</td>
 *    <td>C/A</td>
 *    <td>C2C</td>
 *    <td>L2C</td>
 *    <td>D2C</td>
 *    <td>S2C</td>
 *  </tr>
 *  <tr>
 *    <td>L1 (C/A) + (P2-P1) (semi-codeless)</td>
 *    <td>C2D</td>
 *    <td>L2D</td>
 *    <td>D2D</td>
 *    <td>S2D</td>
 *  </tr>
 *  <tr>
 *    <td>L2C (M)</td>
 *    <td>C2S</td>
 *    <td>L2S</td>
 *    <td>D2S</td>
 *    <td>S2S</td>
 *  </tr>
 *  <tr>
 *    <td>L2C (L)</td>
 *    <td>C2L</td>
 *    <td>L2L</td>
 *    <td>D2L</td>
 *    <td>S2L</td>
 *  </tr>
 *  <tr>
 *    <td>L2C (M+L)</td>
 *    <td>C2X</td>
 *    <td>L2X</td>
 *    <td>D2X</td>
 *    <td>S2X</td>
 *  </tr>
 *  <tr>
 *    <td>P</td>
 *    <td>C2P</td>
 *    <td>L2P</td>
 *    <td>D2P</td>
 *    <td>S2P</td>
 *  </tr>
 *  <tr>
 *    <td>Z-tracking and similiar (AS on)</td>
 *    <td>C2W</td>
 *    <td>L2W</td>
 *    <td>D2W</td>
 *    <td>S2W</td>
 *  </tr>
 *  <tr>
 *    <td>Y</td>
 *    <td>C2Y</td>
 *    <td>L2Y</td>
 *    <td>D2Y</td>
 *    <td>S2Y</td>
 *  </tr>
 *  <tr>
 *    <td>M</td>
 *    <td>C2M</td>
 *    <td>L2M</td>
 *    <td>D2M</td>
 *    <td>S2M</td>
 *  </tr>
 *  <tr>
 *    <td>codeless</td>
 *    <td></td>
 *    <td>L2N</td>
 *    <td>D2N</td>
 *    <td>S2N</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">L5/1176.45</td>
 *    <td>I</td>
 *    <td>C5I</td>
 *    <td>L5I</td>
 *    <td>D5I</td>
 *    <td>S5I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C5Q</td>
 *    <td>L5Q</td>
 *    <td>D5Q</td>
 *    <td>S5Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C5X</td>
 *    <td>L5X</td>
 *    <td>D5X</td>
 *    <td>S5X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="7">GLONASS</td>
 *    <td rowspan="2">G1/1602+k*9/16 k= -7....+12</td>
 *    <td>C/A</td>
 *    <td>C1C</td>
 *    <td>L1C</td>
 *    <td>D1C</td>
 *    <td>S1C</td>
 *  </tr>
 *  <tr>
 *    <td>P</td>
 *    <td>C1P</td>
 *    <td>L1P</td>
 *    <td>D1P</td>
 *    <td>S1P</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="2">G2/1246+k*716</td>
 *    <td>C/A (GLONASS M)</td>
 *    <td>C2C</td>
 *    <td>L2C</td>
 *    <td>D2C</td>
 *    <td>S2C</td>
 *  </tr>
 *  <tr>
 *    <td>P</td>
 *    <td>C2P</td>
 *    <td>L2P</td>
 *    <td>D2P</td>
 *    <td>S2P</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">G3 / 1202.025</td>
 *    <td>I</td>
 *    <td>C3I</td>
 *    <td>L3I</td>
 *    <td>D3I</td>
 *    <td>S3I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C3Q</td>
 *    <td>L3Q</td>
 *    <td>D3Q</td>
 *    <td>S3Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C3X</td>
 *    <td>L3X</td>
 *    <td>D3X</td>
 *    <td>S3X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="19">Galileo</td>
 *    <td rowspan="5">E1 / 1575.42</td>
 *    <td>A PRS</td>
 *    <td>C1A</td>
 *    <td>L1A</td>
 *    <td>D1A</td>
 *    <td>S1A</td>
 *  </tr>
 *  <tr>
 *    <td>B I/NAV OS/CS/SoL</td>
 *    <td>C1B</td>
 *    <td>L1B</td>
 *    <td>D1B</td>
 *    <td>S1B</td>
 *  </tr>
 *  <tr>
 *    <td>C no data</td>
 *    <td>C1C</td>
 *    <td>L1C</td>
 *    <td>D1C</td>
 *    <td>S1C</td>
 *  </tr>
 *  <tr>
 *    <td>B+C</td>
 *    <td>C1X</td>
 *    <td>L1X</td>
 *    <td>D1X</td>
 *    <td>S1X</td>
 *  </tr>
 *  <tr>
 *    <td>A+B+C</td>
 *    <td>C1Z</td>
 *    <td>L1Z</td>
 *    <td>D1Z</td>
 *    <td>S1Z</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">E5a / 1176.45</td>
 *    <td>I F/NAV OS</td>
 *    <td>C5I</td>
 *    <td>L5I</td>
 *    <td>D5I</td>
 *    <td>S5I</td>
 *  </tr>
 *  <tr>
 *    <td>Q no data</td>
 *    <td>C5Q</td>
 *    <td>L5Q</td>
 *    <td>D5Q</td>
 *    <td>S5Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C5X</td>
 *    <td>L5X</td>
 *    <td>D5X</td>
 *    <td>S5X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">E5b / 1207.140</td>
 *    <td>I I/NAV OS/CS/SoL</td>
 *    <td>C7I</td>
 *    <td>L7I</td>
 *    <td>D7I</td>
 *    <td>S7I</td>
 *  </tr>
 *  <tr>
 *    <td>Q no data</td>
 *    <td>C7Q</td>
 *    <td>L7Q</td>
 *    <td>D7Q</td>
 *    <td>S7Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C7X</td>
 *    <td>L7X</td>
 *    <td>D7X</td>
 *    <td>S7X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">E5(E5a+E5b) / 1191.795</td>
 *    <td>I</td>
 *    <td>C8I</td>
 *    <td>L8I</td>
 *    <td>D8I</td>
 *    <td>S8I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C8Q</td>
 *    <td>L8Q</td>
 *    <td>D8Q</td>
 *    <td>S8Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C8X</td>
 *    <td>L8X</td>
 *    <td>D8X</td>
 *    <td>S8X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="5">E6 / 1278.75</td>
 *    <td>A PRS</td>
 *    <td>C6A</td>
 *    <td>L6A</td>
 *    <td>D6A</td>
 *    <td>S6A</td>
 *  </tr>
 *  <tr>
 *    <td>B C/NAV CS</td>
 *    <td>C6B</td>
 *    <td>L6B</td>
 *    <td>D6B</td>
 *    <td>S6B</td>
 *  </tr>
 *  <tr>
 *    <td>C no data</td>
 *    <td>C6C</td>
 *    <td>L6C</td>
 *    <td>D6C</td>
 *    <td>S6C</td>
 *  </tr>
 *  <tr>
 *    <td>B+C</td>
 *    <td>C6X</td>
 *    <td>L6X</td>
 *    <td>D6X</td>
 *    <td>S6X</td>
 *  </tr>
 *  <tr>
 *    <td>A+B+C</td>
 *    <td>C6Z</td>
 *    <td>L6Z</td>
 *    <td>D6Z</td>
 *    <td>S6Z</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="4">SBAS</td>
 *    <td rowspan="1">L1 / 1575.42</td>
 *    <td>C/A</td>
 *    <td>C1C</td>
 *    <td>L1C</td>
 *    <td>D1C</td>
 *    <td>S1C</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">L5 / 1176.45</td>
 *    <td>I</td>
 *    <td>C5I</td>
 *    <td>L5I</td>
 *    <td>D5I</td>
 *    <td>S5I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C5Q</td>
 *    <td>L5Q</td>
 *    <td>D5Q</td>
 *    <td>S5Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C5X</td>
 *    <td>L5X</td>
 *    <td>D5X</td>
 *    <td>S5X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="14">QZSS</td>
 *    <td rowspan="5">L1 / 1575.42</td>
 *    <td>C/A</td>
 *    <td>C1C</td>
 *    <td>L1C</td>
 *    <td>D1C</td>
 *    <td>S1C</td>
 *  </tr>
 *  <tr>
 *    <td>L1C (D)</td>
 *    <td>C1S</td>
 *    <td>L1S</td>
 *    <td>D1S</td>
 *    <td>S1S</td>
 *  </tr>
 *  <tr>
 *    <td>L1C (P)</td>
 *    <td>C1L</td>
 *    <td>L1L</td>
 *    <td>D1L</td>
 *    <td>S1L</td>
 *  </tr>
 *  <tr>
 *    <td>L1C (D+P)</td>
 *    <td>C1X</td>
 *    <td>L1X</td>
 *    <td>D1X</td>
 *    <td>S1X</td>
 *  </tr>
 *  <tr>
 *    <td>L1-SAIF</td>
 *    <td>C1Z</td>
 *    <td>L1Z</td>
 *    <td>D1Z</td>
 *    <td>S1Z</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">L2 / 1227.60</td>
 *    <td>L2C (M)</td>
 *    <td>C2S</td>
 *    <td>L2S</td>
 *    <td>D2S</td>
 *    <td>S2S</td>
 *  </tr>
 *  <tr>
 *    <td>L2C (L)</td>
 *    <td>C2L</td>
 *    <td>L2L</td>
 *    <td>D2L</td>
 *    <td>S2L</td>
 *  </tr>
 *  <tr>
 *    <td>L2C (M+L)</td>
 *    <td>C2X</td>
 *    <td>L2X</td>
 *    <td>D2X</td>
 *    <td>S2X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">L5 / 1176.45</td>
 *    <td>I</td>
 *    <td>C5I</td>
 *    <td>L5I</td>
 *    <td>D5I</td>
 *    <td>S5I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C5Q</td>
 *    <td>L5Q</td>
 *    <td>D5Q</td>
 *    <td>S5Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C5X</td>
 *    <td>L5X</td>
 *    <td>D5X</td>
 *    <td>S5X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">LEX(6) / 1278.75</td>
 *    <td>S</td>
 *    <td>C6S</td>
 *    <td>L6S</td>
 *    <td>D6S</td>
 *    <td>S6S</td>
 *  </tr>
 *  <tr>
 *    <td>L</td>
 *    <td>C6L</td>
 *    <td>L6L</td>
 *    <td>D6L</td>
 *    <td>S6L</td>
 *  </tr>
 *  <tr>
 *    <td>S+L</td>
 *    <td>C6X</td>
 *    <td>L6X</td>
 *    <td>D6X</td>
 *    <td>S6X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="9">BDS</td>
 *    <td rowspan="3">B1 / 1561.098</td>
 *    <td>I</td>
 *    <td>C1I</td>
 *    <td>L1I</td>
 *    <td>D1I</td>
 *    <td>S1I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C1Q</td>
 *    <td>L1Q</td>
 *    <td>D1Q</td>
 *    <td>S1Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C1X</td>
 *    <td>L1X</td>
 *    <td>D1X</td>
 *    <td>S1X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">B2 / 1207.14</td>
 *    <td>I</td>
 *    <td>C2I</td>
 *    <td>L2I</td>
 *    <td>D2I</td>
 *    <td>S2I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C2Q</td>
 *    <td>L2Q</td>
 *    <td>D2Q</td>
 *    <td>S2Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C2X</td>
 *    <td>L2X</td>
 *    <td>D2X</td>
 *    <td>S2X</td>
 *  </tr>
 *  <tr>
 *    <td rowspan="3">B3 / 1268.52</td>
 *    <td>I</td>
 *    <td>C3I</td>
 *    <td>L3I</td>
 *    <td>D3I</td>
 *    <td>S3I</td>
 *  </tr>
 *  <tr>
 *    <td>Q</td>
 *    <td>C3Q</td>
 *    <td>L3Q</td>
 *    <td>D3Q</td>
 *    <td>S3Q</td>
 *  </tr>
 *  <tr>
 *    <td>I+Q</td>
 *    <td>C3X</td>
 *    <td>L3X</td>
 *    <td>D3X</td>
 *    <td>S3X</td>
 *  </tr>
 * </table>
 *
 * (1) Note that for some functions (i.e str2satsys) a whitespace char is interpreted
 *     as the GPS satellite system. This is used for backwards compatibility with 
 *     RINEX version < 3.
 * (2) Mixed satellite is used in RINEX files.<br>
 * (3) These two are not satellite systems but can be used as such to mean an
 *     Unknown or Any satellite system respectively.<br>     
 *
 * @pre       Needs the library datetime::date and library varutils.
 *
 * @copyright No COPYRIGHT whatsoever.
 *
 * <b><center><hr>
 * National Technical University of Athens <br>
 *      Dionysos Satellite Observatory     <br>
 *        Higher Geodesy Laboratory        <br>
 *      http://dionysos.survey.ntua.gr
 * </center></b>
 */

#include <string>
#include <vector>
#include "dates.hpp"

namespace ngpt {

/*! Satellite Systems */
enum class SatelliteSystem : unsigned char
{
  Unknown, /**< denoted by '?' */
  Any,     /**< denoted by '*' */
  Mixed,   /**< denoted by 'M' */
  Gps,     /**< denoted by 'G' */
  Glonass, /**< denoted by 'R' */
  Galileo, /**< denoted by 'E' */
  Qzss,    /**< denoted by 'J' */
  Sbas,    /**< denoted by 'S' */
  BDS      /**< denoted by 'C' */
};

/*! Convert a string to a Satellite System */
SatelliteSystem str2satsys (const std::string&, int&);
/*! Convert a char to a Satellite System */
SatelliteSystem c2satsys (const char&, int&);
/*! convert a Satellite System to a string */
std::string satsys2str (const SatelliteSystem&);
/*! convert a Satellite System to a char */
char satsys2c (const SatelliteSystem&);

/**
 * @brief The frequency band an observation was collected from
 *
 * @details The following table lists the various frequency bands and their
 *          details (@cite rnx302 ).
 *          <table>
 *            <tr>
 *              <th>Band</th>
 *              <th>Freq. Carrier</th>
 *              <th>Satellite System</th>
 *            </tr>
 *            <tr>
 *              <td rowspan="4">1</td>
 *              <td>L1</td>
 *              <td>GPS, QZSS, SBAS</td>
 *            </tr>
 *            <tr>
 *              <td>G1</td>
 *              <td>GLO</td>
 *            </tr>
 *            <tr>
 *              <td>E2-L1-E1</td>
 *              <td>GAL</td>
 *            </tr>
 *            <tr>
 *              <td>B1</td>
 *              <td>BDS</td>
 *            </tr>
 *            <tr>
 *              <td rowspan="2">2</td>
 *              <td>L2</td>
 *              <td>GPS, QZSS</td>
 *            </tr>
 *            <tr>
 *              <td>G2</td>
 *              <td>GLO</td>
 *            </tr>
 *            <tr>
 *              <td rowspan="2">5</td>
 *              <td>L5</td>
 *              <td>GPS, QZSS, SBAS</td>
 *            </tr>
 *            <tr>
 *              <td>E5a</td>
 *              <td>GAL</td>
 *            </tr>
 *            <tr>
 *              <td rowspan="3">6</td>
 *              <td>E6</td>
 *              <td>GAL</td>
 *            </tr>
 *            <tr>
 *              <td>LEX</td>
 *              <td>QZSS</td>
 *            </tr>
 *            <tr>
 *              <td>B3</td>
 *              <td>BDS</td>
 *            </tr>
 *            <tr>
 *              <td rowspan="2">7</td>
 *              <td>E5b</td>
 *              <td>GAL</td>
 *            </tr>
 *            <tr>
 *              <td>B2</td>
 *              <td>BDS</td>
 *            </tr>
 *            <tr>
 *              <td>8</td>
 *              <td>E5a+b</td>
 *              <td>GAL</td>
 *            </tr>
 *            <tr>
 *              <td>0</td>
 *              <td>X</td>
 *              <td>All</td>
 *            </tr>
 *          </table>
 *
 */
enum class CarrierBand : unsigned char
{
  /* Virtual                                            */
  Unknown, ///< Unknown frequency band           Band -1
  Any,     ///< Used to match any carrier band.  Band -1
  /* Gps                                                */
  L1,      ///< GPS L1, SBAS L1, QZSS L1.        Band 1
  L2,      ///< GPS L2, QZSS L2.                 Band 2
  L5,      ///< GPS L5, SBAS L5, QZSS L5.        Band 5
  /* Glonass                                            */
  G1,      ///< Glonass G1.                      Band 1
  G2,      ///< Glonass G2.                      Band 2
  G3,      ///< Glonass G3.                      
  /* Galileo                                            */
  E1,      ///< Galileo E1 (E2-L1-E1).           Band 1
  E5a,     ///< Galileo E5a.                     Band 5
  E5b,     ///< Galileo E5b.                     Band 7
  E5,      ///< Galileo E5 (E5a+E5b).            Band 8
  E6,      ///< Galileo E6.                      Band 6
  /* Qzss                                               */
  LEX,     ///< QZSS LEX(6).                     Band 6
  /* DeiDou                                             */
  B1,      ///< BeiDou B1.                       Band 1
  B2,      ///< BeiDou B2.                       Band 7
  B3,      ///< BeiDou B3.                       Band 6
  /*                                                    */
  Zero     ///< Used with the channel.           Band 0
};

/*! Transform a Carrier Band enumarator to std::string */
std::string carrierband2str (const CarrierBand&);
/*! Transform string to a valid Carrier Band enumarator */
CarrierBand str2carrierband (const std::string&, int&);
/*! Given an integer (i.e. band) and a satellite system resolve the carrier band */
CarrierBand int2carrierband (const SatelliteSystem&,const int&,int&);

/**
 * @brief The type of observation collected
 */
enum class ObservationType : unsigned char
{
  Unknown, ///< Unknown observation type             '?'
  Any,     ///< Used to match any observation type.  '*'
  Range,   ///< Pseudorange, in meters               'C'
  Phase,   ///< Accumulated phase, in meters         'L'
  Doppler, ///< Doppler, in Hz.                      'D'
  SNR,     ///< Signal strength, in dB-Hz.           'S'
  Channel, ///< Channel number.                      'X'
  Iono     ///< Ionospheric phase delay.             'I'
};

/*! Transform an Observation Type enumarator to std::string */
std::string obstype2str (const ObservationType&);
/*! Transform an Observation Type enumarator to char */
char obstype2c (const ObservationType&);
/*! Transform an std::string to a vaild Observation Type enumarator */
ObservationType str2obstype (const std::string&, int&);
/*! Transform a char to a vaild Observation Type enumarator */
ObservationType c2obstype (const char&, int&);

/**
 * @brief The code used to collect the observation or attribute (see @cite rnx302 ).
 * 
 * @attention @cite rnx302 Table A2 : Some channels may have ambiguous identity.
 *            E.g. channel 'S' can be M channel (L2C GPS, QZSS) or D channel (GPS, QZSS)
 *
 * @note  In RINEX v3.xx, the attribute can be left blank if not known.
 *
 */
enum class TrackingCode : unsigned char
{
  Unknown, ///< For types I and X (all) or Unknown tracking code.
  Any,     ///< Used to match any tracking code.
  P,       ///< P code-based (GPS,GLO).
  C,       ///< C code-based (SBAS,GPS,GLO,QZSS) or C channel (GAL).
  D,       ///< D semi-codeless (GPS).
  Y,       ///< Y code-based (GPS).
  M,       ///< M code-based (GPS).
  N,       ///< codeless (GPS)
  A,       ///< A channel (GAL)
  B,       ///< B channel (GAL)
  I,       ///< I channel (GPS,GAL, QZSS, BDS).
  Q,       ///< Q channel (GPS,GAL, QZSS, BDS)
  S,       ///< M channel (L2C GPS, QZSS) or D channel (GPS, QZSS)
  L,       ///< L channel (L2C GPS, QZSS) or P channel (GPS, QZSS)
  X,       ///< B+C channels (GAL) I+Q channels (GPS,GAL, QZSS,BDS) M+L channels (GPS, QZSS) D+P channels (GPS, QZSS)
  W,       ///< Based on Z-tracking (GPS)
  Z        ///< A+B+C channels (GAL)
};

/*! Transform a Tracking Code enumarator to std::string */
std::string trackingcode2str (const TrackingCode&);
/*! Transform a Tracking Code enumarator to a char */
char trackingcode2c (const TrackingCode&);
/*! Transform an std::string to a Tracking Code enumarator */
TrackingCode str2trackingcode (const std::string&, int&);
/*! Transform a char to a Tracking Code enumarator */
TrackingCode c2trackingcode (const char&, int&);

/* @details Satellite Antennae; IGS Codes-20 columns
 *          see ftp://igscb.jpl.nasa.gov/pub/station/general/rcvr_ant.tab
 *          records: Satellite Antennae
 */
enum class SatelliteBlockModel : unsigned char
{
  Unknown,     ///< Unknown block model
  BLOCK_I,     ///< GPS Block I
  BLOCK_II,    ///< GPS Block II
  BLOCK_IIA,   ///< GPS Block IIA
  BLOCK_IIR_A, ///< GPS Block IIR
  BLOCK_IIR_B, ///< GPS Block IIR
  BLOCK_IIR_M, ///< GPS Block IIR-M
  BLOCK_IIF,   ///< GPS Block IIF
  BLOCK_IIIA,  ///< GPS Block IIIA
  GALILEO_0A,  ///< Galileo In-Orbit Validation Element A (GIOVE-A)
  GALILEO_0B,  ///< Galileo In-Orbit Validation Element B (GIOVE-B)
  GALILEO_1,   ///< Galileo IOV
  GLONASS,     ///< GLONASS
  GLONASS_M,   ///< GLONASS-M
  GLONASS_K1,  ///< GLONASS-K1
  GLONASS_K2,  ///< GLONASS-K2
  QZSS         ///< QZSS
};

/*! Convert a block model to a string (strict igs) */
std::string satblockmodel2str(const SatelliteBlockModel&, int&);
/*! Convert a string to a satellite block model (strict igs) */
SatelliteBlockModel str2satblockmodel(const std::string&, int&);

/**
 * @brief Difference level of gnss observables
 * @todo This enum is unfinished;maybe define difference level with respect to satellites or receivers
 */
enum class DifferenceLevel : unsigned char
{
  Unknown, ///< Unknown
  Zero,    ///< Zero-difference observables, undifferenced
  Single,  ///< Single-differenced; two receivers one satellite
  Double,  ///< Double-differenced
  Triple   ///< Triple-differenced
};

/*! Difference level to string */
std::string diflevel2string (const DifferenceLevel&);
/*! Difference level from string */
DifferenceLevel string2diflevel (const std::string&,int&);

}; /* end of namespace */

#endif
