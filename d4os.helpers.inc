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

define('UUID_ZERO', '00000000-0000-0000-0000-000000000000');
global $grid_url;
global $base_url;
$grid_uri   = variable_get('d4os_default_grid_uri', $base_url);
$grid_port  = variable_get('d4os_default_grid_port', '9000');
$grid_url   = $grid_uri. ':'. $grid_port;

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

function d4os_uuid_create() {
  return sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
  mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ),
  mt_rand( 0, 0x0fff ) | 0x4000,
  mt_rand( 0, 0x3fff ) | 0x8000,
  mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ) );
}

function d4os_grid_is_online() {
  // 8002 = user; 8003 = asset+grid+inventory; 8006 = messaging
  // -111 = connection refused
  global $base_url;
  $online = FALSE;

  // check if the user server is online
  $user_server_uri = variable_get('d4os_default_user_server_uri', $base_url). ':'. variable_get('d4os_default_user_server_port', '8002');
  $response = drupal_http_request($user_server_uri);
  if ($response->code == 404 && $response->headers['Server'] == 'Tiny WebServer') {
    $online = TRUE;
  }
  /*global $grid_url;
  $url = $grid_url. '/simstatus/';
  $response = drupal_http_request(variable_get('d4os_ui_stats_status_url',  $url));
  $online = FALSE;
  if ($response->code != 200) {
    $online = FALSE;
  }
  if (strtolower($response->data) == 'ok') {
    $online = TRUE;
  }*/
  return $online;
}

/**
 * AJAX form handler.
 */
function d4os_ajax_form_handler() {
  // The form is generated in an include file which we need to include manually.
  include_once 'modules/node/node.pages.inc';
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];

  // Get the form from the cache.
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);

  // We need to process the form, prepare for that by setting a few internals.
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  // Build, validate and if possible, submit the form.
  drupal_process_form($form_id, $form, $form_state);
  // If validation fails, force form submission.
  if (form_get_errors()) {
    form_execute_handlers('submit', $form, $form_state);
  }

  // This call recreates the form relying solely on the form_state that the
  // drupal_process_form set up.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  return $form;
}

/**
 * Merges any number of arrays / parameters recursively, replacing
 * entries with string keys with values from latter arrays.
 * If the entry or the next value to be assigned is an array, then it
 * automagically treats both arguments as an array.
 * Numeric entries are appended, not replaced, but only if they are
 * unique
 *
 * calling: result = array_merge_recursive_distinct(a1, a2, ... aN)
**/

function d4os_array_merge_recursive_distinct () {
  $arrays = func_get_args();
  $base = array_shift($arrays);
  if(!is_array($base)) $base = empty($base) ? array() : array($base);
  foreach($arrays as $append) {
    if(!is_array($append)) $append = array($append);
    foreach($append as $key => $value) {
      if(!array_key_exists($key, $base) and !is_numeric($key)) {
        $base[$key] = $append[$key];
        continue;
      }
      if(is_array($value) or is_array($base[$key])) {
        $base[$key] = d4os_array_merge_recursive_distinct($base[$key], $append[$key]);
      } else if(is_numeric($key)) {
        if(!in_array($value, $base)) $base[] = $value;
      } else {
        $base[$key] = $value;
      }
    }
  }
  return $base;
}