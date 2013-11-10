<?php

/**
 * @package     d4os_main
 * @subpackage  d4os_helpers
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
/**
 * UUID zero
 */
if (!defined('UUID_ZERO')) {
  define('UUID_ZERO', '00000000-0000-0000-0000-000000000000');
}

/**
 * @global String $grid_url
 * @name $grid_url
 */
global $grid_url;

/**
 * @global String $base_url
 * @name $base_url
 */
global $base_url;

/**
 * @global Integer $grid_is_online
 * @name $grid_is_online
 */
global $grid_is_online;

$grid_uri = variable_get('d4os_default_grid_uri', $base_url);
$grid_port = variable_get('d4os_default_grid_port', '8002');
$grid_path = variable_get('d4os_default_grid_path', '');
$grid_url = $grid_uri . ':' . $grid_port. $grid_path;

/**
 * Helpfull function to search items in an array with a lot of sublevels
 * @param Mixed The item to search
 * @param Array The array to crawl
 * @return Mixed The key of the main array where the item is or nothing
 */
function d4os_array_search_recursive($needle, $haystack) {
  foreach ($haystack as $k => $v) {
    foreach ($v as $v2) {
      if ($v2 === $needle) {
        return $k;
        break;
      }
    }
  }
}

/**
 * Utility used to help the services logs in the watchdog.
 * @param String The name of the module sending the message
 * @param Array An array contining infos to send
 * Like array('type'=>'string', 'name'=>'function call', 'data'=>$data);
 * @param Int The watchdog severity level
 */
function d4os_log($module, $array, $severity = WATCHDOG_DEBUG) {
  $output = '';
  $output .= "\n*************************************************************\n";
  $output .= format_date(time(), 'custom', 'Y-m-d G:i:s') . "\n";
  foreach ($array as $value) {
    switch ($value['type']) {
      case 'string':
        switch ($value['name']) {
          case 'function call':
          case 'function output':
            $output .= ' ' . $value['name'] . ' = ' . $value['data'] . "\n-------------------------------------------------------------\n";
            break;
          default:
            $output .= ' ' . $value['name'] . ' = ' . $value['data'] . "\n";
            break;
        }
        break;
      case 'var':
        $output .= var_export($value['data'], TRUE) . "\n";
        break;
      case 'array':
        $output .= print_r($value['data'], TRUE);
        break;
    }
  }

  $path = variable_get('d4os_default_debug_file_path', '');
  if (!empty($path)) {
    file_put_contents($path, $output, FILE_APPEND);
  }
}

/**
 * Generates a uuid
 * @return String The uuid
 */
function d4os_uuid_create() {
  return sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x', mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0x0fff) | 0x4000, mt_rand(0, 0x3fff) | 0x8000, mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff));
}

/**
 * Check if the grid is online
 * @return Bool True or False
 */
function d4os_grid_is_online() {
  global $grid_is_online;
  // 8002 = user; 8003 = asset+grid+inventory; 8006 = messaging
  // -111 = connection refused
  global $base_url;

  // If we have not checked recently, poll it, otherwise use the current value.
  // Coz this can be a slow operation if the web server is not on the grid server.
  // Measured it at 0.8 seconds on my test server.
  // Also, if it's a high traffic web server, and the online status is on all pages, pointless checking many times a second.
  $grid_is_online_timestamp = variable_get('d4os_grid_is_online_timestamp', 0);
  $time = time();
  if ($grid_is_online_timestamp < ($time - (60))) {
    $online = FALSE;
    $url = variable_get('d4os_default_grid_uri', $base_url);// TODO : why not use loginuri ?
    $port = variable_get('d4os_default_grid_port', '8002');
    $path = variable_get('d4os_default_grid_path', '');
    $url = $url.$path;

    // remove http://
    if (strpos($url, 'http://') !== FALSE) {
      $url = substr($url, 7);
    }

    $fp = @fsockopen($url, $port, $errno, $errstr, 4.0);
    if ($fp) {
      $online = TRUE;
    }
    variable_set('d4os_grid_is_online_timestamp', $time);
    variable_set('d4os_grid_is_online', $online);
  }
  else {
    $online = variable_get('d4os_grid_is_online');
  }
  $grid_is_online = $online;
  return $online;
}

/**
 * AJAX form handler. Used for the ahah forms
 * @return Array An array containing the Drupal form
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
 * */
function d4os_array_merge_recursive_distinct() {
  $arrays = func_get_args();
  $base = array_shift($arrays);
  if (!is_array($base))
    $base = empty($base) ? array() : array($base);
  foreach ($arrays as $append) {
    if (!is_array($append))
      $append = array($append);
    foreach ($append as $key => $value) {
      if (!array_key_exists($key, $base) and !is_numeric($key)) {
        $base[$key] = $append[$key];
        continue;
      }
      if (is_array($value) or is_array($base[$key])) {
        $base[$key] = d4os_array_merge_recursive_distinct($base[$key], $append[$key]);
      }
      elseif (is_numeric($key)) {
        if (!in_array($value, $base))
          $base[] = $value;
      }
      else {
        $base[$key] = $value;
      }
    }
  }
  return $base;
}
