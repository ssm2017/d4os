<?php
/**
 * @package   d4os_ui_users
 * @copyright Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

function d4os_ui_users_user_generate($username, $lastname, $password, $mail, $home=0) {
  global $base_url;

  $password_salt = md5(microtime() . mt_rand(0,0xffff));
  return array(
    'grid' => array(

      'account' => array(
        'uuid'                => d4os_uuid_create(),
        'username'            => $username,
        'lastname'            => $lastname,
        'created'             => time(),
        'user_inventory_uri'  => variable_get('d4os_default_inventory_server_uri', $base_url). ':'. variable_get('d4os_default_inventory_server_port', '8004'),
        'user_asset_uri'      => variable_get('d4os_default_asset_server_uri', $base_url). ':'. variable_get('d4os_default_asset_server_port', '8003'),
        'login_level'         => variable_get('d4os_ui_users_default_god_level', 10),
        'email'               => $mail,
      ),

      'auth' => array(
        'password_salt'   => $password_salt,
        'password_hash'   => md5(md5($password) . ":" . $password_salt),
        'simian_md5hash'  => '$1$'. md5($password),
        'simian_a1hash'   => md5($username. ' '. $lastname. ':Inventory:'. $password),
      ),

      'presence' => array(
        'home_region'     => $home,
        'home_location_x' => 128,
        'home_location_y' => 128,
        'home_location_z' => 128,
        'home_look_at_x'  => 100,
        'home_look_at_y'  => 100,
        'home_look_at_z'  => 100,
        'last_login'      => 0,
      ),

      'profile' => array(
        'can_do_mask'   => 0,
        'want_do_mask'  => 0,
        'about_text'    => variable_get('d4os_ui_users_default_profile_about_text', 'I am a new user'),
        'first_text'    => variable_get('d4os_ui_users_default_profile_first_text', 'Stuf.'),
        'image'         => sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x', 0, 0, 0, 0, 0, 0, 0, 0 ),
        'first_image'   => sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x', 0, 0, 0, 0, 0, 0, 0, 0 ),
      ),
    )
  );
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
  $maxlength = ($type == 'username') ? USERNAME_MAX_LENGTH : LASTNAME_MAX_LENGTH;
  if (strlen($name) > $maxlength) return t('The !name %name is too long: it must be %max characters or less.', array('!name'=>$type_str, '%name' => $name, '%max' => $maxlength));
  if (count(explode(' ', $name)) > 1) return t('You can not use spaces in !name', array('!name'=>$type_str));
  return FALSE;
}

function d4os_ui_users_get_god_roles() {
  $roles = array(
    0   => variable_get('d4os_ui_users_god_level_roles_0',0),
    10  => variable_get('d4os_ui_users_god_level_roles_10',0),
    20  => variable_get('d4os_ui_users_god_level_roles_20',0),
    30  => variable_get('d4os_ui_users_god_level_roles_30',0),
    40  => variable_get('d4os_ui_users_god_level_roles_40',0),
    50  => variable_get('d4os_ui_users_god_level_roles_50',0),
    60  => variable_get('d4os_ui_users_god_level_roles_60',0),
    70  => variable_get('d4os_ui_users_god_level_roles_70',0),
    80  => variable_get('d4os_ui_users_god_level_roles_80',0),
    90  => variable_get('d4os_ui_users_god_level_roles_90',0),
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

function d4os_ui_users_get_god_level($roles = array()) {
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

function d4os_ui_users_get_grid_fields() {
  return array(
    // account
    'UUID',
    'username',
    'lastname',
    'created',
    'godLevel',
    'email',
    'userInventoryURI',
    'userAssetURI',

    // auth
    'passwordSalt',
    'passwordHash',

    // presence
    'homeRegion',
    'homeLocationX',
    'homeLocationY',
    'homeLocationZ',
    'homeLookAtX',
    'homeLookAtY',
    'homeLookAtZ',
    'lastLogin',

    // profile
    'profileCanDoMask',
    'profileWantDoMask',
    'profileAboutText',
    'profileFirstText',
    'profileImage',
    'profileFirstImage',
  );
}

function d4os_ui_users_clean_account(&$account) {
  unset(
    // account
    $account->grid_UUID,
    $account->grid_username,
    $account->grid_lastname,
    $account->grid_created,
    $account->grid_godLevel,
    $account->grid_email,
    $account->grid_userInventoryURI,
    $account->grid_userAssetURI,

    // auth
    $account->grid_passwordSalt,
    $account->grid_passwordHash,

    // presence
    $account->grid_homeRegion,
    $account->grid_homeLocationX,
    $account->grid_homeLocationY,
    $account->grid_homeLocationZ,
    $account->grid_homeLookAtX,
    $account->grid_homeLookAtY,
    $account->grid_homeLookAtZ,
    $account->grid_lastLogin,

    // profile
    $account->grid_profileCanDoMask,
    $account->grid_profileWantDoMask,
    $account->grid_profileAboutText,
    $account->grid_profileFirstText,
    $account->grid_profileImage,
    $account->grid_profileFirstImage,
    $account->grid_profileURL,
    $account->grid_profilePartner
  );
}

function d4os_ui_users_account_to_grid($account) {
  $grid_account = array();

  // account
  if (isset($account['d4os']['grid']['account']['uuid']))               $grid_account['UUID']             = $account['d4os']['grid']['account']['uuid'];
  if (isset($account['d4os']['grid']['account']['username']))           $grid_account['username']         = $account['d4os']['grid']['account']['username'];
  if (isset($account['d4os']['grid']['account']['lastname']))           $grid_account['lastname']         = $account['d4os']['grid']['account']['lastname'];
  if (isset($account['d4os']['grid']['account']['created']))            $grid_account['created']          = $account['d4os']['grid']['account']['created'];
  if (isset($account['d4os']['grid']['account']['login_level']))        $grid_account['godLevel']         = $account['d4os']['grid']['account']['login_level'];
  if (isset($account['d4os']['grid']['account']['email']))              $grid_account['email']            = $account['d4os']['grid']['account']['email'];
  if (isset($account['d4os']['grid']['account']['user_inventory_uri'])) $grid_account['userInventoryURI'] = $account['d4os']['grid']['account']['user_inventory_uri'];
  if (isset($account['d4os']['grid']['account']['user_asset_uri']))     $grid_account['userAssetURI']     = $account['d4os']['grid']['account']['user_asset_uri'];
  
  // auth
  if (isset($account['d4os']['grid']['auth']['password_salt']))   $grid_account['passwordSalt'] = $account['d4os']['grid']['auth']['password_salt'];
  if (isset($account['d4os']['grid']['auth']['password_hash']))   $grid_account['passwordHash'] = $account['d4os']['grid']['auth']['password_hash'];
  if (isset($account['d4os']['grid']['auth']['simian_md5hash']))  $grid_account['md5hash']      = $account['d4os']['grid']['auth']['simian_md5hash'];
  if (isset($account['d4os']['grid']['auth']['simian_a1hash']))   $grid_account['a1hash']       = $account['d4os']['grid']['auth']['simian_a1hash'];
  
  // presence
  if (isset($account['d4os']['grid']['presence']['home_region']))     $grid_account['homeRegion']     = $account['d4os']['grid']['presence']['home_region'];
  if (isset($account['d4os']['grid']['presence']['home_location_x'])) $grid_account['homeLocationX']  = $account['d4os']['grid']['presence']['home_location_x'];
  if (isset($account['d4os']['grid']['presence']['home_location_y'])) $grid_account['homeLocationY']  = $account['d4os']['grid']['presence']['home_location_y'];
  if (isset($account['d4os']['grid']['presence']['home_location_z'])) $grid_account['homeLocationZ']  = $account['d4os']['grid']['presence']['home_location_z'];
  if (isset($account['d4os']['grid']['presence']['home_look_at_x']))  $grid_account['homeLookAtX']    = $account['d4os']['grid']['presence']['home_look_at_x'];
  if (isset($account['d4os']['grid']['presence']['home_look_at_y']))  $grid_account['homeLookAtY']    = $account['d4os']['grid']['presence']['home_look_at_y'];
  if (isset($account['d4os']['grid']['presence']['home_look_at_z']))  $grid_account['homeLookAtZ']    = $account['d4os']['grid']['presence']['home_look_at_z'];
  if (isset($account['d4os']['grid']['presence']['last_login']))      $grid_account['lastLogin']      = $account['d4os']['grid']['presence']['last_login'];
  
  //profile
  if (isset($account['d4os']['grid']['profile']['can_do_mask']))  $grid_account['profileCanDoMask']   = $account['d4os']['grid']['profile']['can_do_mask'];
  if (isset($account['d4os']['grid']['profile']['want_do_mask'])) $grid_account['profileWantDoMask']  = $account['d4os']['grid']['profile']['want_do_mask'];
  if (isset($account['d4os']['grid']['profile']['about_text']))   $grid_account['profileAboutText']   = $account['d4os']['grid']['profile']['about_text'];
  if (isset($account['d4os']['grid']['profile']['first_text']))   $grid_account['profileFirstText']   = $account['d4os']['grid']['profile']['first_text'];
  if (isset($account['d4os']['grid']['profile']['image']))        $grid_account['profileImage']       = $account['d4os']['grid']['profile']['image'];
  if (isset($account['d4os']['grid']['profile']['first_image']))  $grid_account['profileFirstImage']  = $account['d4os']['grid']['profile']['first_image'];
  if (isset($account['d4os']['grid']['profile']['url']))          $grid_account['profileURL']         = $account['d4os']['grid']['profile']['url'];
  if (isset($account['d4os']['grid']['profile']['partner']))      $grid_account['profilePartner']     = $account['d4os']['grid']['profile']['partner'];

  return $grid_account;
}

function d4os_ui_users_grid_to_account($grid_account) {
  $account = array();

  // account
  if (isset($grid_account['UUID']))              $account['grid']['account']['uuid']               = $grid_account['UUID'];
  if (isset($grid_account['username']))          $account['grid']['account']['username']           = $grid_account['username'];
  if (isset($grid_account['lastname']))          $account['grid']['account']['lastname']           = $grid_account['lastname'];
  if (isset($grid_account['userInventoryURI']))  $account['grid']['account']['user_inventory_uri'] = $grid_account['userInventoryURI'];
  if (isset($grid_account['userAssetURI']))      $account['grid']['account']['user_asset_uri']     = $grid_account['userAssetURI'];
  if (isset($grid_account['created']))           $account['grid']['account']['created']            = $grid_account['created'];
  if (isset($grid_account['godLevel']))          $account['grid']['account']['login_level']        = $grid_account['godLevel'];
  if (isset($grid_account['email']))             $account['grid']['account']['email']              = $grid_account['email'];

  // auth
  if (isset($grid_account['passwordSalt'])) $account['grid']['auth']['password_salt']   = $grid_account['passwordSalt'];
  if (isset($grid_account['passwordHash'])) $account['grid']['auth']['password_hash']   = $grid_account['passwordHash'];
  if (isset($grid_account['md5hash']))      $account['grid']['auth']['simian_md5hash']  = $grid_account['md5hash'];
  if (isset($grid_account['a1hash']))       $account['grid']['auth']['simian_a1hash']   = $grid_account['a1hash'];

  // presence
  if (isset($grid_account['homeRegion']))    $account['grid']['presence']['home_region']     = $grid_account['homeRegion'];
  if (isset($grid_account['homeLocationX'])) $account['grid']['presence']['home_location_x'] = $grid_account['homeLocationX'];
  if (isset($grid_account['homeLocationY'])) $account['grid']['presence']['home_location_y'] = $grid_account['homeLocationY'];
  if (isset($grid_account['homeLocationZ'])) $account['grid']['presence']['home_location_z'] = $grid_account['homeLocationZ'];
  if (isset($grid_account['homeLookAtX']))   $account['grid']['presence']['home_look_at_x']  = $grid_account['homeLookAtX'];
  if (isset($grid_account['homeLookAtY']))   $account['grid']['presence']['home_look_at_y']  = $grid_account['homeLookAtY'];
  if (isset($grid_account['homeLookAtZ']))   $account['grid']['presence']['home_look_at_z']  = $grid_account['homeLookAtZ'];
  if (isset($grid_account['lastLogin']))     $account['grid']['presence']['last_login']      = $grid_account['lastLogin'];

  // profile
  if (isset($grid_account['profileCanDoMask']))   $account['grid']['profile']['can_do_mask']  = $grid_account['profileCanDoMask'];
  if (isset($grid_account['profileWantDoMask']))  $account['grid']['profile']['want_do_mask'] = $grid_account['profileWantDoMask'];
  if (isset($grid_account['profileAboutText']))   $account['grid']['profile']['about_text']   = $grid_account['profileAboutText'];
  if (isset($grid_account['profileFirstText']))   $account['grid']['profile']['first_text']   = $grid_account['profileFirstText'];
  if (isset($grid_account['profileImage']))       $account['grid']['profile']['image']        = $grid_account['profileImage'];
  if (isset($grid_account['profileFirstImage']))  $account['grid']['profile']['first_image']  = $grid_account['profileFirstImage'];
  if (isset($grid_account['profileURL']))         $account['grid']['profile']['url']          = $grid_account['profileURL'];
  if (isset($grid_account['profilePartner']))     $account['grid']['profile']['partner']      = $grid_account['profilePartner'];

  return $account;
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

function d4os_ui_users_validate_email($address) {
  return ( ! preg_match("/^([a-z0-9\+_\-]+)(\.[a-z0-9\+_\-]+)*@([a-z0-9\-]+\.)+[a-z]{2,6}$/ix", $address)) ? FALSE : TRUE;
}
