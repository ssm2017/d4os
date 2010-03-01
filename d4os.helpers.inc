<?php
/**
* @package    d4os
* @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_array_search_recursive($needle, $haystack) {
  foreach ($haystack as $k => $v) {
    foreach($v as $v2) {
      if ($v2 === $needle) {
        return $k;
        break;
      }
    }
  }
}

function d4os_log($module, $array, $severity = WATCHDOG_DEBUG) {
  $output = '';
  foreach ($array as $value) {
    switch ($value['type']) {
      case 'string':
        switch ($value['name']) {
          case 'function call':
          case 'function output':
            $output .= ' '. $value['name']. ' = <strong>'. $value['data']. '</strong><br/>';
            break;
          default:
            $output .= ' '. $value['name']. ' = '. $value['data']. '<br/>';
            break;
        }
        break;
      case 'var':
        $output .= var_export($value['data'], TRUE). '<br />';
        break;
      case 'array':
        $output .= '<pre>'. print_r($value['data'], true). '</pre>';
        break;
    }
  }
  watchdog($module, $output, array(), $severity);
}

function d4os_build_uuid() {
  return sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
  mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ),
  mt_rand( 0, 0x0fff ) | 0x4000,
  mt_rand( 0, 0x3fff ) | 0x8000,
  mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ) );
}