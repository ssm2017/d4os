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
 */

function d4os_io_services_search_dir_places_query($text, $category, $query_start=0) {
  $pieces = split(" ", $text);
  $text = join("%", $pieces);
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms'),
    );
  }

  // do the job
  $params = array(
    'category'  => $category,
    'text'      => $text,
    'start'     => $start,
  );
  return module_invoke_all('os_search_services_dir_places_query', $params);
}

function d4os_io_services_search_dir_places_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_popular_query($flags) {
  // do the job
  $params = array(
    'flags' => $flags,
  );
  return module_invoke_all('os_search_services_dir_popular_query', $params);
}

function d4os_io_services_search_dir_popular_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_land_query($flags, $type, $price, $area, $query_start) {
  // do the job
  $params = array(
    'flags'       => $flags,
    'type'        => $type,
    'price'       => $price,
    'area'        => $area,
    'query_start' => $query_start,
  );
  return module_invoke_all('os_search_services_dir_land_query', $params);
}

function d4os_io_services_search_dir_land_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_events_query($text="", $flags="", $query_start=0) {
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms.'),
    );
  }
  // do the job
  $params = array(
    'text'        => $text,
    'flags'       => $flags,
    'query_start' => $query_start,
  );
  return module_invoke_all('os_search_services_dir_events_query', $params);
}

function d4os_io_services_search_dir_events_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_classified_query($text, $flags, $category, $query_start) {
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms.'),
    );
  }

  // do the job
  $params = array(
    'text'        => $text,
    'flags'       => $flags,
    'category'    => $category,
    'query_start' => $query_start,
  );
  return module_invoke_all('os_search_services_dir_classified_query', $params);
}

function d4os_io_services_search_dir_classified_query_access() {
  return TRUE;
}

function d4os_io_services_search_event_info_query($eventID=0) {
  // do the job
  return module_invoke_all('os_search_services_event_info_query', array('eventID' => $eventID));
}

function d4os_io_services_search_event_info_query_access() {
  return TRUE;
}

function d4os_io_services_search_classifieds_info_query($classifiedID) {
  // do the job
  return module_invoke_all('os_search_services_classifieds_info_query', array('classifiedID' => $classifiedID));
}

function d4os_io_services_search_classifieds_info_query_access() {
  return TRUE;
}