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

function d4os_ui_users_user_generate($username, $lastname, $password, $mail, $home=0) {
  global $base_url;
  $user = array();
  $user['inworld_UUID']               = d4os_build_uuid();
  $user['inworld_username']           = $username;
  $user['inworld_lastname']           = $lastname;
  
  $user['inworld_passwordSalt']       = md5(microtime() . mt_rand(0,0xffff));
  $user['inworld_passwordHash']       = md5(md5($password) . ":" . $user['inworld_passwordSalt']);
  
  $user['inworld_homeRegion']         = $home;
  $user['inworld_homeLocationX']      = 128;
  $user['inworld_homeLocationY']      = 128;
  $user['inworld_homeLocationZ']      = 128;
  $user['inworld_homeLookAtX']        = 100;
  $user['inworld_homeLookAtY']        = 100;
  $user['inworld_homeLookAtZ']        = 100;
  
  $user['inworld_created']            = time();
  $user['inworld_lastLogin']          = 0;
  
  $user['inworld_userInventoryURI']   = variable_get('d4os_default_inventory_server_uri', $base_url);
  $user['inworld_userAssetURI']       = variable_get('d4os_default_asset_server_uri', $base_url);
  
  $user['inworld_profileCanDoMask']   = 0;
  $user['inworld_profileWantDoMask']  = 0;
  $user['inworld_profileAboutText']   = variable_get('d4os_ui_users_default_profile_about_text', 'I am a new user');
  $user['inworld_profileFirstText']   = variable_get('d4os_ui_users_default_profile_first_text', 'Stuf.');
  $user['inworld_profileImage']       = sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x', 0, 0, 0, 0, 0, 0, 0, 0 );
  $user['inworld_profileFirstImage']  = sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x', 0, 0, 0, 0, 0, 0, 0, 0 );
  $user['inworld_godLevel']           = variable_get('d4os_ui_users_default_god_level', 10);
  $user['inworld_email']              = $mail;
  
  return $user;
}

function d4os_ui_users_validate_name($name, $type) {
  if (!strlen($name)) return t('You must enter a !name.', array('!name'=>t($type)));
  if (substr($name, 0, 1) == ' ') return t('The !name cannot begin with a space.', array('!name'=>t($type)));
  if (substr($name, -1) == ' ') return t('The !name cannot end with a space.', array('!name'=>t($type)));
  if (strpos($name, '  ') !== FALSE) return t('The !name cannot contain multiple spaces in a row.', array('!name'=>t($type)));
  if (ereg("[^\x80-\xF7 [:alnum:]@_.-]", $name)) return t('The !name contains an illegal character.', array('!name'=>t($type)));
  if (preg_match('/[\x{80}-\x{A0}'.          // Non-printable ISO-8859-1 + NBSP
                   '\x{AD}'.                 // Soft-hyphen
                   '\x{2000}-\x{200F}'.      // Various space characters
                   '\x{2028}-\x{202F}'.      // Bidirectional text overrides
                   '\x{205F}-\x{206F}'.      // Various text hinting characters
                   '\x{FEFF}'.               // Byte order mark
                   '\x{FF01}-\x{FF60}'.      // Full-width latin
                   '\x{FFF9}-\x{FFFD}'.      // Replacement characters
                   '\x{0}]/u',               // NULL byte
                   $name)) {
    return t('The !name contains an illegal character.', array('!name'=>t($type)));
  }
  if (strpos($name, '@') !== FALSE && !eregi('@([0-9a-z](-?[0-9a-z])*.)+[a-z]{2}([zmuvtg]|fo|me)?$', $name)) return t('The !name is not a valid authentication ID.', array('!name'=>t($type)));
  $maxlength = ($type == 'username') ? USERNAME_MAX_LENGTH : LASTNAME_MAX_LENGTH;
  if (strlen($name) > $maxlength) return t('The !name %name is too long: it must be %max characters or less.', array('!name'=>t($type), '%name' => $name, '%max' => $maxlength));
  if (count(explode(' ', $name)) > 1) return t('You can not use spaces in !name', array('!name'=>t($type)));
  return FALSE;
}

function d4os_ui_users_get_god_roles() {
  $roles = array(
    0  => variable_get('d4os_ui_users_god_level_roles_0',0),
    10 => variable_get('d4os_ui_users_god_level_roles_10',0),
    20 => variable_get('d4os_ui_users_god_level_roles_20',0),
    30 => variable_get('d4os_ui_users_god_level_roles_30',0),
    40 => variable_get('d4os_ui_users_god_level_roles_40',0),
    50 => variable_get('d4os_ui_users_god_level_roles_50',0),
    60 => variable_get('d4os_ui_users_god_level_roles_60',0),
    70 => variable_get('d4os_ui_users_god_level_roles_70',0),
    80 => variable_get('d4os_ui_users_god_level_roles_80',0),
    90 => variable_get('d4os_ui_users_god_level_roles_90',0),
    100 => variable_get('d4os_ui_users_god_level_roles_100',0),
    200 => variable_get('d4os_ui_users_god_level_roles_200',0),
  );
  return $roles;
}

function d4os_ui_users_get_god_role_by_role($role) {
  // get the level
  $levels = array_keys(d4os_ui_users_get_god_roles(), $role);
  // get the higher level
  rsort($levels);
  return isset($levels[0]) ? $levels[0] : FALSE;
}

function d4os_ui_users_get_god_level($roles) {
  if (!count($roles)) return variable_get('d4os_ui_users_default_god_level', 10);
  $levels = array();
  foreach ($roles as $k=>$v) {
    $level = d4os_ui_users_get_god_role_by_role($k);
    if ($level) $levels[] = $level;
    if ($level === 0) return 0;
  }
  rsort($levels);
  return isset($levels[0]) ? $levels[0] : FALSE;
}

function d4os_ui_users_get_inworld_fields() {
  return array(
    'UUID',
    'username',
    'lastname',
    'passwordSalt',
    'passwordHash',
    'homeRegion',
    'homeLocationX',
    'homeLocationY',
    'homeLocationZ',
    'homeLookAtX',
    'homeLookAtY',
    'homeLookAtZ',
    'created',
    'lastLogin',
    'userInventoryURI',
    'userAssetURI',
    'profileCanDoMask',
    'profileWantDoMask',
    'profileAboutText',
    'profileFirstText',
    'profileImage',
    'profileFirstImage',
    'godLevel',
    'email'
  );
}

function d4os_ui_users_account_to_inworld_array($account) {
  $inworld_user = array();
  foreach ($account as $key => $value) {
    if ((substr($key, 0, 8) == 'inworld_')) {
      $new_key = substr($key, 8);
      $inworld_user[$new_key] = $value;
    }
  }
  return $inworld_user;
}

function d4os_ui_users_generate_mail($uuid) {
  global $base_url;
  $domain = $base_url;
  $replacers = array(
    'http://'   => '',
    'https://'  => '',
  );
  foreach ($replacers as $key=>$value) {
    $domain = str_replace($key, $value, $domain);
  }
  $string = variable_get('d4os_ui_users_default_missing_email', '[uuid]@[livesite]');
  $replacers = array(
    '[uuid]'      =>$uuid,
    '[livesite]'  =>$domain,
  );
  foreach ($replacers as $key=>$value) {
    $string = str_replace($key, $value, $string);
  }
  return $string;
}