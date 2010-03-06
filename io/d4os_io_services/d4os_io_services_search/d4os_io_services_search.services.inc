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

/*
 * The code in this script was inspired and mostly copied from ossearch
 * http://forge.opensimulator.org/gf/project/ossearch
 * Copyright (c)Melanie Thielker (http://opensimulator.org/)
 */

global $d4os_io_services_search_log_level;
$d4os_io_services_search_log_level = variable_get('d4os_io_services_search_log_level', 0);

function d4os_io_services_search_dir_places_query($text, $category, $query_start=0) {
  global $d4os_io_services_profile_log_level;

  $pieces = split(" ", $text);
  $text = join("%", $pieces);
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms'),
    );
  }

  $params = array(
    'category'  => $category,
    'text'      => $text,
    'start'     => $start,
  );

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'dir_places_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_dir_places_query', $params);

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'dir_places_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_dir_places_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_popular_query($flags) {
  global $d4os_io_services_profile_log_level;

  $params = array(
    'flags' => $flags,
  );

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'dir_popular_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_dir_popular_query', $params);

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'dir_popular_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_dir_popular_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_land_query($flags, $type, $price, $area, $query_start) {
  global $d4os_io_services_profile_log_level;

  $params = array(
    'flags'       => $flags,
    'type'        => $type,
    'price'       => $price,
    'area'        => $area,
    'query_start' => $query_start,
  );

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'dir_land_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_dir_land_query', $params);

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'dir_land_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_dir_land_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_events_query($text="", $flags="", $query_start=0) {
  global $d4os_io_services_profile_log_level;
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms.'),
    );
  }

  $params = array(
    'text'        => $text,
    'flags'       => $flags,
    'query_start' => $query_start,
  );

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'dir_events_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_dir_events_query', $params);

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'dir_events_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_dir_events_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_classified_query($text, $flags, $category, $query_start) {
  global $d4os_io_services_profile_log_level;

  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms.'),
    );
  }

  $params = array(
    'text'        => $text,
    'flags'       => $flags,
    'category'    => $category,
    'query_start' => $query_start,
  );

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'dir_classified_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_dir_classified_query', $params);

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'dir_classified_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_dir_classified_query_access() {
  return TRUE;
}

function d4os_io_services_search_event_info_query($eventID=0) {
  global $d4os_io_services_profile_log_level;

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'event_info_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_event_info_query', array('eventID' => $eventID));

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'event_info_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_event_info_query_access() {
  return TRUE;
}

function d4os_io_services_search_classifieds_info_query($classifiedID) {
  global $d4os_io_services_profile_log_level;

  // log
  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'classifieds_info_query',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_search_services_classifieds_info_query', array('classifiedID' => $classifiedID));

  if ($d4os_io_services_search_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'classifieds_info_query',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_services_search', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_search_classifieds_info_query_access() {
  return TRUE;
}