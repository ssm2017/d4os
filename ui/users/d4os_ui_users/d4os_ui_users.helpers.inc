<?php
/**
 * @package   d4os_ui_users
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Generate a user
 */
function d4os_ui_users_generate_user($firstname, $lastname, $password, $mail, $home=UUID_ZERO, $default_model=UUID_ZERO) {

  $password_salt = md5(microtime() . mt_rand(0,0xffff));
  $user_uuid = d4os_uuid_create();

  // build the user base object
  $grid_user                  = new stdClass();

  // account
  $grid_user->PrincipalID  = $user_uuid;
  $grid_user->ScopeID      = UUID_ZERO;
  $grid_user->FirstName    = $firstname;
  $grid_user->LastName     = $lastname;
  $grid_user->Email        = $mail;
  $grid_user->ServiceURLs  = 'HomeURI= GatekeeperURI= InventoryServerURI= AssetServerURI=';
  $grid_user->Created      = time();
  $grid_user->UserLevel    = variable_get('d4os_ui_users_default_user_level', 0);
  $grid_user->UserFlags    = 0;
  $grid_user->UserTitle    = 0;

  // auth
  $grid_user->UUID          = $user_uuid;
  $grid_user->passwordHash  = md5(md5($password) . ":" . $password_salt);
  $grid_user->passwordSalt  = $password_salt;
  $grid_user->webLoginKey   = UUID_ZERO;
  $grid_user->accountType   = 'UserAccount';

  // grid_user
  $grid_user->UserID         = $user_uuid;
  $grid_user->HomeRegionID   = ($home != UUID_ZERO) ? $home : variable_get('d4os_ui_users_default_home_region',0);
  $grid_user->HomePosition   = variable_get('d4os_ui_users_default_home_location', '<128,128,128>');
  $grid_user->HomeLookAt     = variable_get('d4os_ui_users_default_home_look_at', '<100,100,100>');
  $grid_user->LastRegionID   = $grid_user->HomeRegionID;
  $grid_user->LastPosition   = $grid_user->HomePosition;
  $grid_user->LastLookAt     = $grid_user->HomeLookAt;
  $grid_user->Online         = FALSE;
  $grid_user->Login          = 0;
  $grid_user->Logout         = 0;

  // others
  $grid_user->defaultModel = $default_model;
  $grid_user->defaultGroup = variable_get('d4os_ui_users_default_group',UUID_ZERO);

  return $grid_user;
}

function d4os_ui_users_validate_name($name, $type, $type_str) {
  if (!strlen($name)) return t('You must enter a !name.', array('!name'=>$type_str));
  if (substr($name, 0, 1) == ' ') return t('The !name cannot begin with a space.', array('!name'=>$type_str));
  if (substr($name, -1) == ' ') return t('The !name cannot end with a space.', array('!name'=>$type_str));
  if (strpos($name, '  ') !== FALSE) return t('The !name cannot contain multiple spaces in a row.', array('!name'=>$type_str));
  if (ereg("[^\x80-\xF7 [:alnum:]@_.-]", $name)) return t('The !name contains an illegal character.', array('!name'=>$type_str));
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
    return t('The !name contains an illegal character.', array('!name'=>$type_str));
  }
  if (strpos($name, '@') !== FALSE && !eregi('@([0-9a-z](-?[0-9a-z])*.)+[a-z]{2}([zmuvtg]|fo|me)?$', $name)) return t('The !name is not a valid authentication ID.', array('!name'=>$type_str));
  $maxlength = ($type == 'firstname') ? FIRSTNAME_MAX_LENGTH : LASTNAME_MAX_LENGTH;
  if (strlen($name) > $maxlength) return t('The !name %name is too long: it must be %max characters or less.', array('!name'=>$type_str, '%name' => $name, '%max' => $maxlength));
  if (count(explode(' ', $name)) > 1) return t('You can not use spaces in !name', array('!name'=>$type_str));
  return FALSE;
}

function d4os_ui_users_get_user_level_roles() {
  $roles = array(
    -1  => variable_get('d4os_ui_users_user_level_roles_blocked',0),
    0   => variable_get('d4os_ui_users_user_level_roles_0',0),
    1   => variable_get('d4os_ui_users_user_level_roles_1',0),
    20  => variable_get('d4os_ui_users_user_level_roles_20',0),
    30  => variable_get('d4os_ui_users_user_level_roles_30',0),
    40  => variable_get('d4os_ui_users_user_level_roles_40',0),
    50  => variable_get('d4os_ui_users_user_level_roles_50',0),
    60  => variable_get('d4os_ui_users_user_level_roles_60',0),
    70  => variable_get('d4os_ui_users_user_level_roles_70',0),
    80  => variable_get('d4os_ui_users_user_level_roles_80',0),
    90  => variable_get('d4os_ui_users_user_level_roles_90',0),
    100 => variable_get('d4os_ui_users_user_level_roles_100',0),
    150 => variable_get('d4os_ui_users_user_level_roles_150',0),
    200 => variable_get('d4os_ui_users_user_level_roles_200',0),
    250 => variable_get('d4os_ui_users_user_level_roles_250',0),
  );
  return $roles;
}

function d4os_ui_users_get_user_level_role_by_role($role) {
  // get the level
  $levels = array_keys(d4os_ui_users_get_user_level_roles(), $role);
  // get the higher level
  rsort($levels);
  return isset($levels[0]) ? $levels[0] : 0;
}

function d4os_ui_users_get_user_level($roles = array()) {
  if (!count($roles)) return variable_get('d4os_ui_users_default_user_level', 0);
  $levels = array();
  foreach ($roles as $k=>$v) {
    $level = d4os_ui_users_get_user_level_role_by_role($k);
    if ($level) $levels[] = $level;
    if ($level === -1) return -1;
  }
  rsort($levels);
  return isset($levels[0]) ? $levels[0] : 0;
}

function d4os_ui_users_validate_email($address) {
  return ( ! preg_match("/^([a-z0-9\+_\-]+)(\.[a-z0-9\+_\-]+)*@([a-z0-9\-]+\.)+[a-z]{2,6}$/ix", $address)) ? FALSE : TRUE;
}
