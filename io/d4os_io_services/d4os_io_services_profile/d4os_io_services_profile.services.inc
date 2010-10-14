<?php
/**
 * @package    d4os_io_services_profile
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * The code in this script was inspired and mostly copied from osprofile
 * @link http://forge.opensimulator.org/gf/project/osprofile
 * Copyright (c)Melanie Thielker (http://opensimulator.org/)
 */

global $d4os_io_services_profile_log_level;
$d4os_io_services_profile_log_level = variable_get('d4os_io_services_profile_log_level', 0);

function d4os_io_services_profile_avatarclassifiedsrequest($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatarclassifiedsrequest',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatarclassifiedsrequest', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatarclassifiedsrequest',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_avatarclassifiedsrequest_access() {
  return TRUE;
}

function d4os_io_services_profile_classified_update($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'classified_update',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_classified_update', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'classified_update',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_classified_update_access() {
  return TRUE;
}

function d4os_io_services_profile_classified_delete($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'classified_delete',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_classified_delete', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'classified_delete',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_classified_delete_access() {
  return TRUE;
}

function d4os_io_services_profile_avatarpicksrequest($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatarpicksrequest',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatarpicksrequest', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatarpicksrequest',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_avatarpicksrequest_access() {
  return TRUE;
}

function d4os_io_services_profile_pickinforequest($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'pickinforequest',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_pickinforequest', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'pickinforequest',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_pickinforequest_access() {
  return TRUE;
}

function d4os_io_services_profile_picks_update($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'picks_update',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_picks_update', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'picks_update',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_picks_update_access() {
  return TRUE;
}

function d4os_io_services_profile_picks_delete($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'picks_delete',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_picks_delete', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'picks_delete',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_picks_delete_access() {
  return TRUE;
}

function d4os_io_services_profile_avatarnotesrequest($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatarnotesrequest',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatarnotesrequest', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatarnotesrequest',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_avatarnotesrequest_access() {
  return TRUE;
}

function d4os_io_services_profile_avatar_notes_update($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatar_notes_update',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatar_notes_update', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatar_notes_update',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_avatar_notes_update_access() {
  return TRUE;
}

function d4os_io_services_profile_avatar_properties_request($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatar_properties_request',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatar_properties_request', $params);

  //if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatar_properties_request',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  //}
  return $return;
}

function d4os_io_services_profile_avatar_properties_request_access() {
  return TRUE;
}

function d4os_io_services_profile_avatar_properties_update($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatar_properties_update',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatar_properties_update', $params);

  //if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatar_properties_update',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  //}
  return $return;
}

function d4os_io_services_profile_avatar_properties_update_access() {
  return TRUE;
}

function d4os_io_services_profile_avatar_interests_request($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatar_interests_request',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatar_interests_request', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatar_interests_request',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_avatar_interests_request_access() {
  return TRUE;
}

function d4os_io_services_profile_avatar_interests_update($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'avatar_interests_update',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_avatar_interests_update', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'avatar_interests_update',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_avatar_interests_update_access() {
  return TRUE;
}

function d4os_io_services_profile_user_preferences_request($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'user_preferences_request',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_user_preferences_request', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'user_preferences_request',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_user_preferences_request_access() {
  return TRUE;
}

function d4os_io_services_profile_user_preferences_update($params) {
  global $d4os_io_services_profile_log_level;
  // log
  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'user_preferences_update',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }

  // do the job
  $return = module_invoke_all('os_profile_services_user_preferences_update', $params);

  if ($d4os_io_services_profile_log_level > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'user_preferences_update',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_io_svc_prof', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_profile_user_preferences_update_access() {
  return TRUE;
}
