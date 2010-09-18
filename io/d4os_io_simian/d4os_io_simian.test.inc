<?php
/**
 * @package    d4os_io_simian
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

function d4os_io_simian_test_form() {
  $form = array();
  d4os_io_simian_test_asset($form);
  d4os_io_simian_test_inventory($form);
  d4os_io_simian_test_user($form);
  d4os_io_simian_test_grid($form);
  d4os_io_simian_test_generic($form);
  return $form;
}

function d4os_io_simian_test_asset(&$form) {
  $form['d4os_io_simian_test']['asset'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Asset'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
// AddAsset
  $form['d4os_io_simian_test']['asset']['AddAsset'] = array(
    '#type'         => 'fieldset',
    '#title'        => 'AddAsset',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
    '#description'  => t('Create or replace an asset.'),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['AssetID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'AssetID',
    '#description'  => t('UUID of the asset'),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['CreatorID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'CreatorID',
    '#description'  => t('UUID of the asset creator'),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['Asset'] = array(
    '#type'         => 'textarea',
    '#title'        => 'Asset',
    '#description'  => t('Raw asset data'),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['Temporary'] = array(
    '#type'         => 'checkbox',
    '#title'        => 'Temporary',
    '#description'  => t('True if this is a temporary asset'),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['Public'] = array(
    '#type'         => 'checkbox',
    '#title'        => 'Public',
    '#description'  => t('True if this asset is publicly accessible (without authorization)'),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Go'),
    '#ahah'   => array(
      'path'    => 'grid/simian/test/AddAsset/js',
      'wrapper' => 'd4os-simian-admin-add-asset-output',
      'method'  => 'replace',
      'effect'  => 'fade',
    ),
  );
  $form['d4os_io_simian_test']['asset']['AddAsset']['output'] = array(
    '#title'  => t('Response'),
    '#type'   => 'item',
    '#prefix' => '<div id="d4os-simian-admin-add-asset-output">',
    '#suffix' => '</div>',
  );

// GetAsset

// RemoveAsset
  
}

function d4os_io_simian_test_inventory(&$form) {
  // AddInventory
  
  // AddInventoryFolder
  
  // AddInventoryItem
  
  // GetFolderForType
  
  // GetInventoryNode
  
  // GetLibrary
  
  // MoveInventoryNodes
  
  // PurgeInventoryFolder
  
  // RemoveInventoryNode
  
}

function d4os_io_simian_test_user(&$form) {
  $form['d4os_io_simian_test']['user'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('User'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
// AddCapability
  $form['d4os_io_simian_test']['user']['AddCapability'] = array(
    '#type'         => 'fieldset',
    '#title'        => 'AddCapability',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
    '#description'  => t('Create or update a capability resource.'),
  );
  $form['d4os_io_simian_test']['user']['AddCapability']['CapabilityID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'CapabilityID',
    '#description'  => t('UUID of the capability to create. Can be used to update a capability'),
  );
  $form['d4os_io_simian_test']['user']['AddCapability']['OwnerID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'OwnerID',
    '#description'  => t('UUID of the owner of this capability'),
  );
  $form['d4os_io_simian_test']['user']['AddCapability']['Resource'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Resource',
    '#description'  => t('URL of the protected resource this capability maps to'),
  );
  $form['d4os_io_simian_test']['user']['AddCapability']['Expiration'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Expiration',
    '#description'  => t('UTC timestamp when this capability will expire'),
  );
  $form['d4os_io_simian_test']['user']['AddCapability']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Go'),
    '#ahah'   => array(
      'path'    => 'grid/simian/test/AddCapability/js',
      'wrapper' => 'd4os-simian-admin-add-capability-output',
      'method'  => 'replace',
      'effect'  => 'fade',
    ),
  );
  $form['d4os_io_simian_test']['user']['AddCapability']['output'] = array(
    '#title'  => t('Response'),
    '#type'   => 'item',
    '#prefix' => '<div id="d4os-simian-admin-add-capability-output">',
    '#suffix' => '</div>',
  );

// AddIdentity
  $form['d4os_io_simian_test']['user']['AddIdentity'] = array(
    '#type'         => 'fieldset',
    '#title'        => 'AddIdentity',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
    '#description'  => t('Create or update an identity and associate it with an existing user account. User accounts can have multiple identities (login methods) associated with them.'),
  );
  $form['d4os_io_simian_test']['user']['AddIdentity']['Identifier'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Identifier',
    '#description'  => t('Identifier, for example a login name'),
  );
  $form['d4os_io_simian_test']['user']['AddIdentity']['Credential'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Credential',
    '#description'  => t('Credential, for example a hashed password'),
  );
  $form['d4os_io_simian_test']['user']['AddIdentity']['Type'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Type',
    '#description'  => t('Identity type, such as "md5hash"'),
  );
  $form['d4os_io_simian_test']['user']['AddIdentity']['UserID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'UserID',
    '#description'  => t('UUID of the user to associate the identity with'),
  );
  $form['d4os_io_simian_test']['user']['AddIdentity']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Go'),
    '#ahah'   => array(
      'path'    => 'grid/simian/test/AddIdentity/js',
      'wrapper' => 'd4os-simian-admin-add-identty-output',
      'method'  => 'replace',
      'effect'  => 'fade',
    ),
  );
  $form['d4os_io_simian_test']['user']['AddIdentity']['output'] = array(
    '#title'  => t('Response'),
    '#type'   => 'item',
    '#prefix' => '<div id="d4os-simian-admin-add-identity-output">',
    '#suffix' => '</div>',
  );

// AddSession
  $form['d4os_io_simian_test']['user']['AddSession'] = array(
    '#type'         => 'fieldset',
    '#title'        => 'AddSession',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
    '#description'  => t('Creates a login session for a user.'),
  );
  $form['d4os_io_simian_test']['user']['AddSession']['UserID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'UserID',
    '#description'  => t('UUID of the user to create a login session for'),
  );
  $form['d4os_io_simian_test']['user']['AddSession']['SessionID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'SessionID',
    '#description'  => t('Identifier to use for this session'),
  );
  $form['d4os_io_simian_test']['user']['AddSession']['SecureSessionID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'SecureSessionID',
    '#description'  => t('Secure identifier to use for this session. Must only be transmitted across secure channels'),
  );
  $form['d4os_io_simian_test']['user']['AddSession']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Go'),
    '#ahah'   => array(
      'path'    => 'grid/simian/test/AddSession/js',
      'wrapper' => 'd4os-simian-admin-add-session-output',
      'method'  => 'replace',
      'effect'  => 'fade',
    ),
  );
  $form['d4os_io_simian_test']['user']['AddSession']['output'] = array(
    '#title'  => t('Response'),
    '#type'   => 'item',
    '#prefix' => '<div id="d4os-simian-admin-add-session-output">',
    '#suffix' => '</div>',
  );

// AddUser
  $form['d4os_io_simian_test']['user']['AddUser'] = array(
    '#type'         => 'fieldset',
    '#title'        => 'AddUser',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
    '#description'  => t('Create or update a user account.'),
  );
  $form['d4os_io_simian_test']['user']['AddUser']['UserID'] = array(
    '#type'         => 'textfield',
    '#title'        => 'UserID',
    '#description'  => t('UUID of the user to create or update'),
  );
  $form['d4os_io_simian_test']['user']['AddUser']['Name'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Name',
    '#description'  => t('Account name'),
  );
  $form['d4os_io_simian_test']['user']['AddUser']['Email'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Email',
    '#description'  => t('E-mail address associated with this account'),
  );
  $form['d4os_io_simian_test']['user']['AddUser']['AccessLevel'] = array(
    '#type'         => 'textfield',
    '#title'        => 'AccessLevel',
    '#description'  => t('Value from 0-255 indicating the access level of this account (higher implies more access)'),
  );
  $form['d4os_io_simian_test']['user']['AddUser']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Go'),
    '#ahah'   => array(
      'path'    => 'grid/simian/test/AddUser/js',
      'wrapper' => 'd4os-simian-admin-add-user-output',
      'method'  => 'replace',
      'effect'  => 'fade',
    ),
  );
  $form['d4os_io_simian_test']['user']['AddUser']['output'] = array(
    '#title'  => t('Response'),
    '#type'   => 'item',
    '#prefix' => '<div id="d4os-simian-admin-add-user-output">',
    '#suffix' => '</div>',
  );

// AddUserData
  
  // AutorizeIdentity
  
  // GetIdentities
  
  // GetSession
  
  // GetUser
  
  // GetUsers
  
  // RemoveCapability
  
  // RemoveIdentity
  
  // RemoveSession
  
  // RemoveSessions
  
  // RemoveUser
  
  // RemoveUserCapabilities
  
  // UpdateSession
}

function d4os_io_simian_test_grid(&$form) {
  // AddScene
  
  // GetScene
  
  // GetScenes
  
  // RemoveScene
}

function d4os_io_simian_test_generic(&$form) {
  // AddGeneric
  
  // GetGenerics
  
  // RemoveGeneric
}


function d4os_io_simian_test_js($command) {
  // Prevent duplicate wrappers.
  unset($track_form['#prefix'], $track_form['#suffix']);
  //$field = 'd4os_oss_'. $service. '_'. $method;
  //$cmd = $_POST[$field];
  //if ($cmd == 'none' || $cmd == '') {
  //  $answer = theme('d4os_oss_wrongbutton');
  //}
  //else {
  //  $answer = theme('d4os_oss_answer', d4os_oss_service_call($service, $method, $cmd));
  //}
  $answer = '<pre>'. print_r(d4os_io_simian_call($command, d4os_io_simian_get_args($command)), true). '</pre>';
  $output = theme('status_messages') . $answer;

  $javascript = drupal_add_js(NULL, NULL);
  if (isset($javascript['setting'])) {
    $output .= '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>';
  }
  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));
}

function d4os_io_simian_test_get_args($command) {
  $args = array();
  switch ($command) {
// regions
    case 'admin_create_region':
      $args['region_id']              = $_POST['d4os_io_simian_admin_create_region_region_id']; // str : optional
      $args['region_name']            = $_POST['d4os_io_simian_admin_create_region_region_name']; // str
      $args['region_master_first']    = $_POST['d4os_io_simian_admin_create_region_region_master_first']; // str
      $args['region_master_last']     = $_POST['d4os_io_simian_admin_create_region_region_master_last']; // str
      $args['region_master_uuid']     = $_POST['d4os_io_simian_admin_create_region_region_master_uuid'];
      $args['region_master_password'] = $_POST['d4os_io_simian_admin_create_region_region_master_password']; // str
      $args['listen_ip']              = $_POST['d4os_io_simian_admin_create_region_listen_ip']; // str
      $args['listen_port']            = $_POST['d4os_io_simian_admin_create_region_listen_port']; // int
      $args['external_address']       = $_POST['d4os_io_simian_admin_create_region_external_address']; // str
      $args['region_x']               = $_POST['d4os_io_simian_admin_create_region_region_x']; // int
      $args['region_y']               = $_POST['d4os_io_simian_admin_create_region_region_y']; // int
      $args['persist']                = $_POST['d4os_io_simian_admin_create_region_persist']; // bool : optional
      $args['public']                 = $_POST['d4os_io_simian_admin_create_region_public']; // bool : optional
      $args['enable_voice']           = $_POST['d4os_io_simian_admin_create_enable_voice']; // bool : optional
      break;
    case 'admin_delete_region':
      $args['region_name'] = $_POST['d4os_io_simian_admin_delete_region_region_name']; // str
      break;
    // TODO : add close region
    case 'admin_modify_region':
      $args['region_id']      = $_POST['d4os_io_simian_admin_modify_region_region_id'];
      $args['region_name']    = $_POST['d4os_io_simian_admin_modify_region_region_name'];
      $args['public']         = $_POST['d4os_io_simian_admin_modify_region_public'];
      $args['enable_voice']   = $_POST['d4os_io_simian_admin_modify_enable_voice'];
      break;
    case 'admin_region_query':
      $args['region_uuid'] = $_POST['d4os_io_simian_admin_region_query_region_uuid'];
      $args['region_name'] = $_POST['d4os_io_simian_admin_region_query_region_name'];
      break;
    case 'admin_shutdown':
      $delay = $_POST['d4os_io_simian_admin_shutdown_delayed'];
      if (!empty($delay)) {
        $args['shutdown'] = 'delayed';
        $args['milliseconds'] = $delay*1000;
      }
      break;
    case 'admin_broadcast':
      $args['message'] = $_POST['d4os_io_simian_admin_broadcast_message'];
      break;
    case 'admin_restart':
      $args['regionID'] = $_POST['d4os_io_simian_admin_restart_regionID'];
      break;
    case 'admin_load_heightmap':
      $args['filename'] = $_POST['d4os_io_simian_admin_load_heightmap_filename'];
      $args['regionid'] = $_POST['d4os_io_simian_admin_load_heightmap_regionid'];
      break;
// users
    case 'admin_create_user':
      $args['user_firstname'] = $_POST['d4os_io_simian_admin_create_user_user_firstname'];
      $args['user_lastname']  = $_POST['d4os_io_simian_admin_create_user_user_lastname'];
      $args['user_password']  = $_POST['d4os_io_simian_admin_create_user_user_password'];
      $args['start_region_x'] = $_POST['d4os_io_simian_admin_create_user_start_region_x'];
      $args['start_region_y'] = $_POST['d4os_io_simian_admin_create_user_start_region_y'];
      break;
    case 'admin_create_user_email':
      $args['user_firstname'] = $_POST['d4os_io_simian_admin_create_user_email_user_firstname'];
      $args['user_lastname']  = $_POST['d4os_io_simian_admin_create_user_email_user_lastname'];
      $args['user_password']  = $_POST['d4os_io_simian_admin_create_user_email_user_password'];
      $args['start_region_x'] = $_POST['d4os_io_simian_admin_create_user_email_start_region_x'];
      $args['start_region_y'] = $_POST['d4os_io_simian_admin_create_user_email_start_region_y'];
      break;
    case 'admin_exists_user':
      $args['user_firstname'] = $_POST['d4os_io_simian_admin_exists_user_user_firstname'];
      $args['user_lastname']  = $_POST['d4os_io_simian_admin_exists_user_user_lastname'];
      break;
    case 'admin_update_user':
      $args['user_firstname'] = $_POST['d4os_io_simian_admin_update_user_user_firstname'];
      $args['user_lastname']  = $_POST['d4os_io_simian_admin_update_user_user_lastname'];
      $args['user_password']  = $_POST['d4os_io_simian_admin_update_user_user_password'];
      $args['start_region_x'] = $_POST['d4os_io_simian_admin_update_user_start_region_x'];
      $args['start_region_y'] = $_POST['d4os_io_simian_admin_update_user_start_region_y'];
      break;
// region state
    case 'admin_load_xml':
      $args['filename']     = $_POST['d4os_io_simian_admin_load_xml_filename'];
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_load_xml_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_load_xml_region_name'];
      $args['xml_version']  = $_POST['d4os_io_simian_admin_load_xml_xml_version'];
      break;
    case 'admin_save_xml':
      $args['filename']     = $_POST['d4os_io_simian_admin_save_xml_filename'];
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_save_xml_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_save_xml_region_name'];
      $args['xml_version']  = $_POST['d4os_io_simian_admin_save_xml_xml_version'];
      break;
    case 'admin_load_oar':
      $args['filename']     = $_POST['d4os_io_simian_admin_load_oar_filename'];
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_load_oar_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_load_oar_region_name'];
      break;
    case 'admin_save_oar':
      $args['filename']     = $_POST['d4os_io_simian_admin_save_oar_filename'];
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_save_oar_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_save_oar_region_name'];
      break;
// acl
    case 'admin_acl_clear':
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_acl_clear_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_acl_clear_region_name']; // not working : failed to switch to region 00000000-0000-0000-0000-000000000000
      break;
    case 'admin_acl_add': // not working : Cannot cast from source type to destination type.
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_acl_add_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_acl_add_region_name']; // not working : failed to switch to region 00000000-0000-0000-0000-000000000000
      $args['users']        = explode(';', $_POST['d4os_io_simian_admin_acl_add_users']);
      break;
    case 'admin_acl_remove': // not working : Cannot cast from source type to destination type.
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_acl_remove_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_acl_remove_region_name']; // not working : failed to switch to region 00000000-0000-0000-0000-000000000000
      $args['users']        = explode(';', $_POST['d4os_io_simian_admin_acl_remove_users']);
      break;
    case 'admin_acl_list': // not working : return empty array (users)
      $args['region_uuid']  = $_POST['d4os_io_simian_admin_acl_list_region_uuid'];
      $args['region_name']  = $_POST['d4os_io_simian_admin_acl_list_region_name']; // not working : failed to switch to region 00000000-0000-0000-0000-000000000000
      break;
  }
  return $args;
}

function d4os_io_simian_test_call($command, $args) {
  global $grid_url;
  $args['password'] = variable_get('d4os_io_simian_default_password', 'mypw');
  $data = array();
  if (module_exists('d4os_oss') && variable_get('d4os_io_simian_use_oss', 1)) {
    $uuid = variable_get('d4os_oss_default_opensim_uuid', 'e978e986-61a9-4d93-a40a-c5e9e05047d0');
    $data['result'] = xmlrpc(d4os_oss_build_url(), 'simian_proxy', $uuid, $command, $args);
  }
  else {
    $data['result'] = xmlrpc($grid_url, $command, $args);
  }
  $data['error'] = xmlrpc_error_msg();
  $data['errno'] = xmlrpc_errno();
  return $data;
}
