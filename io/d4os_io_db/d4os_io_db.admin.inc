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

function d4os_io_db_admin() {
  $bases = d4os_io_db_build_bases_list();

  $form = array();

  $form['d4os_io_db_type'] = array(
    '#type'           => 'radios',
    '#title'          => t('Database system'),
    '#options'        => array(t('Same as website'), t('Single'), t('Multiple')),
    '#default_value'  => variable_get('d4os_io_db_type', 0),
    '#description'    => t('Select if you are using only one database for all the OpenSim grid or if you are using separated bases.'),
  );
// single
  $form['single_base'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Single base'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
    '#description'    => t('Fill these values if you are using a single database.'),
  );
  $form['single_base']['d4os_io_db_single_host'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host'),
    '#default_value'  => variable_get('d4os_io_db_single_host', 'localhost'),
    '#description'    => t('Host'),
  );
  $form['single_base']['d4os_io_db_single_db'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Database'),
    '#default_value'  => variable_get('d4os_io_db_single_db', 'opensim'),
    '#description'    => t('Database'),
  );
  $form['single_base']['d4os_io_db_single_prefix'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Prefix'),
    '#default_value'  => variable_get('d4os_io_db_single_prefix', ''),
    '#description'    => t('Prefix'),
  );
  $form['single_base']['d4os_io_db_single_username'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Username'),
    '#default_value'  => variable_get('d4os_io_db_single_username', 'root'),
    '#description'    => t('Username'),
  );
  $form['single_base']['d4os_io_db_single_password'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Password'),
    '#default_value'  => variable_get('d4os_io_db_single_password', 'root'),
    '#description'    => t('Password'),
  );
  
// multiple
  $form['multiple_base'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Multiple bases'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
    '#description'    => t('Fill these values if you are using multiple databases.'),
  );
  
// asset
  $form['multiple_base']['asset'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Asset'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );
  $options = $bases;
  unset($options['os_asset']);
  $form['multiple_base']['asset']['d4os_io_db_multiple_asset_same'] = array(
    '#type'           => 'select',
    '#title'          => t('Same as'),
    '#default_value'  => variable_get('d4os_io_db_multiple_asset_same', 'none'),
    '#options'        => $options,
    '#description'    => t('Select if this database is the same as another one.'),
  );
  $form['multiple_base']['asset']['d4os_io_db_multiple_asset_host'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host'),
    '#default_value'  => variable_get('d4os_io_db_multiple_asset_host', 'localhost'),
    '#description'    => t('Host'),
  );
  $form['multiple_base']['asset']['d4os_io_db_multiple_asset_db'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Database'),
    '#default_value'  => variable_get('d4os_io_db_multiple_asset_db', 'asset'),
    '#description'    => t('Database'),
  );
  $form['multiple_base']['asset']['d4os_io_db_multiple_asset_prefix'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Prefix'),
    '#default_value'  => variable_get('d4os_io_db_multiple_asset_prefix', ''),
    '#description'    => t('Prefix'),
  );
  $form['multiple_base']['asset']['d4os_io_db_multiple_asset_username'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Username'),
    '#default_value'  => variable_get('d4os_io_db_multiple_asset_username', 'root'),
    '#description'    => t('Username'),
  );
  $form['multiple_base']['asset']['d4os_io_db_multiple_asset_password'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Password'),
    '#default_value'  => variable_get('d4os_io_db_multiple_asset_password', 'root'),
    '#description'    => t('Password'),
  );
// auth
  //$form['multiple_base']['auth'] = array(
  //  '#type'           => 'fieldset',
  //  '#title'          => t('Auth'),
  //  '#collapsible'    => TRUE,
  //  '#collapsed'      => TRUE,
  //);
  //$options = $bases;
  //unset($options['os_auth']);
  //$form['multiple_base']['auth']['d4os_io_db_multiple_auth_same'] = array(
  //  '#type'           => 'select',
  //  '#title'          => t('Same as'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_auth_same', 'none'),
  //  '#options'        => $options,
  //  '#description'    => t('Select if this database is the same as another one.'),
  //);
  //$form['multiple_base']['auth']['d4os_io_db_multiple_auth_host'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Host'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_auth_host', 'localhost'),
  //  '#description'    => t('Host'),
  //);
  //$form['multiple_base']['auth']['d4os_io_db_multiple_auth_db'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Database'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_auth_db', 'auth'),
  //  '#description'    => t('Database'),
  //);
  //$form['multiple_base']['auth']['d4os_io_db_multiple_auth_prefix'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Prefix'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_auth_prefix', ''),
  //  '#description'    => t('Prefix'),
  //);
  //$form['multiple_base']['auth']['d4os_io_db_multiple_auth_username'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Username'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_auth_username', 'root'),
  //  '#description'    => t('Username'),
  //);
  //$form['multiple_base']['auth']['d4os_io_db_multiple_auth_password'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Password'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_auth_password', 'root'),
  //  '#description'    => t('Password'),
  //);
// estate
  //$form['multiple_base']['estate'] = array(
  //  '#type'           => 'fieldset',
  //  '#title'          => t('Estate'),
  //  '#collapsible'    => TRUE,
  //  '#collapsed'      => TRUE,
  //);
  //$options = $bases;
  //unset($options['os_estate']);
  //$form['multiple_base']['estate']['d4os_io_db_multiple_estate_same'] = array(
  //  '#type'           => 'select',
  //  '#title'          => t('Same as'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_estate_same', 'none'),
  //  '#options'        => $options,
  //  '#description'    => t('Select if this database is the same as another one.'),
  //);
  //$form['multiple_base']['estate']['d4os_io_db_multiple_estate_host'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Host'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_estate_host', 'localhost'),
  //  '#description'    => t('Host'),
  //);
  //$form['multiple_base']['estate']['d4os_io_db_multiple_estate_db'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Database'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_estate_db', 'estate'),
  //  '#description'    => t('Database'),
  //);
  //$form['multiple_base']['estate']['d4os_io_db_multiple_estate_prefix'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Prefix'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_estate_prefix', ''),
  //  '#description'    => t('Prefix'),
  //);
  //$form['multiple_base']['estate']['d4os_io_db_multiple_estate_username'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Username'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_estate_username', 'root'),
  //  '#description'    => t('Username'),
  //);
  //$form['multiple_base']['estate']['d4os_io_db_multiple_estate_password'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Password'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_estate_password', 'root'),
  //  '#description'    => t('Password'),
  //);
// grid
  //if (module_exists('d4os_ui_regions')) {
    $form['multiple_base']['grid'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Grid'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $options = $bases;
    unset($options['os_grid']);
    $form['multiple_base']['grid']['d4os_io_db_multiple_grid_same'] = array(
      '#type'           => 'select',
      '#title'          => t('Same as'),
      '#default_value'  => variable_get('d4os_io_db_multiple_grid_same', 'none'),
      '#options'        => $options,
      '#description'    => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['grid']['d4os_io_db_multiple_grid_host'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Host'),
      '#default_value'  => variable_get('d4os_io_db_multiple_grid_host', 'localhost'),
      '#description'    => t('Host'),
    );
    $form['multiple_base']['grid']['d4os_io_db_multiple_grid_db'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Database'),
      '#default_value'  => variable_get('d4os_io_db_multiple_grid_db', 'grid'),
      '#description'    => t('Database'),
    );
    $form['multiple_base']['grid']['d4os_io_db_multiple_grid_prefix'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Prefix'),
      '#default_value'  => variable_get('d4os_io_db_multiple_grid_prefix', ''),
      '#description'    => t('Prefix'),
    );
    $form['multiple_base']['grid']['d4os_io_db_multiple_grid_username'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Username'),
      '#default_value'  => variable_get('d4os_io_db_multiple_grid_username', 'root'),
      '#description'    => t('Username'),
    );
    $form['multiple_base']['grid']['d4os_io_db_multiple_grid_password'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Password'),
      '#default_value'  => variable_get('d4os_io_db_multiple_grid_password', 'root'),
      '#description'    => t('Password'),
    );
  //}
// groups
  if (module_exists('d4os_ui_groups') || module_exists('d4os_io_services_groups')) {
    $form['multiple_base']['groups'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Groups'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $options = $bases;
    unset($options['os_groups']);
    $form['multiple_base']['groups']['d4os_io_db_multiple_groups_same'] = array(
      '#type'           => 'select',
      '#title'          => t('Same as'),
      '#default_value'  => variable_get('d4os_io_db_multiple_groups_same', 'none'),
      '#options'        => $options,
      '#description'    => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['groups']['d4os_io_db_multiple_groups_host'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Host'),
      '#default_value'  => variable_get('d4os_io_db_multiple_groups_host', 'localhost'),
      '#description'    => t('Host'),
    );
    $form['multiple_base']['groups']['d4os_io_db_multiple_groups_db'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Database'),
      '#default_value'  => variable_get('d4os_io_db_multiple_groups_db', 'groups'),
      '#description'    => t('Database'),
    );
    $form['multiple_base']['groups']['d4os_io_db_multiple_groups_prefix'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Prefix'),
      '#default_value'  => variable_get('d4os_io_db_multiple_groups_prefix', ''),
      '#description'    => t('Prefix'),
    );
    $form['multiple_base']['groups']['d4os_io_db_multiple_groups_username'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Username'),
      '#default_value'  => variable_get('d4os_io_db_multiple_groups_username', 'root'),
      '#description'    => t('Username'),
    );
    $form['multiple_base']['groups']['d4os_io_db_multiple_groups_password'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Password'),
      '#default_value'  => variable_get('d4os_io_db_multiple_groups_password', 'root'),
      '#description'    => t('Password'),
    );
  }
// inventory
  $form['multiple_base']['inventory'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Inventory'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );
  $options = $bases;
  unset($options['os_inventory']);
  $form['multiple_base']['inventory']['d4os_io_db_multiple_inventory_same'] = array(
    '#type'           => 'select',
    '#title'          => t('Same as'),
    '#default_value'  => variable_get('d4os_io_db_multiple_inventory_same', 'none'),
    '#options'        => $options,
    '#description'    => t('Select if this database is the same as another one.'),
  );
  $form['multiple_base']['inventory']['d4os_io_db_multiple_inventory_host'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host'),
    '#default_value'  => variable_get('d4os_io_db_multiple_inventory_host', 'localhost'),
    '#description'    => t('Host'),
  );
  $form['multiple_base']['inventory']['d4os_io_db_multiple_inventory_db'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Database'),
    '#default_value'  => variable_get('d4os_io_db_multiple_inventory_db', 'inventory'),
    '#description'    => t('Database'),
  );
  $form['multiple_base']['inventory']['d4os_io_db_multiple_inventory_prefix'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Prefix'),
    '#default_value'  => variable_get('d4os_io_db_multiple_inventory_prefix', ''),
    '#description'    => t('Prefix'),
  );
  $form['multiple_base']['inventory']['d4os_io_db_multiple_inventory_username'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Username'),
    '#default_value'  => variable_get('d4os_io_db_multiple_inventory_username', 'root'),
    '#description'    => t('Username'),
  );
  $form['multiple_base']['inventory']['d4os_io_db_multiple_inventory_password'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Password'),
    '#default_value'  => variable_get('d4os_io_db_multiple_inventory_password', 'root'),
    '#description'    => t('Password'),
  );
// messaging
  //$form['multiple_base']['messaging'] = array(
  //  '#type'           => 'fieldset',
  //  '#title'          => t('Messaging'),
  //  '#collapsible'    => TRUE,
  //  '#collapsed'      => TRUE,
  //);
  //$options = $bases;
  //unset($options['os_messaging']);
  //$form['multiple_base']['messaging']['d4os_io_db_multiple_messaging_same'] = array(
  //  '#type'           => 'select',
  //  '#title'          => t('Same as'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_messaging_same', 'none'),
  //  '#options'        => $options,
  //  '#description'    => t('Select if this database is the same as another one.'),
  //);
  //$form['multiple_base']['messaging']['d4os_io_db_multiple_messaging_host'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Host'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_messaging_host', 'localhost'),
  //  '#description'    => t('Host'),
  //);
  //$form['multiple_base']['messaging']['d4os_io_db_multiple_messaging_db'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Database'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_messaging_db', 'messaging'),
  //  '#description'    => t('Database'),
  //);
  //$form['multiple_base']['messaging']['d4os_io_db_multiple_messaging_prefix'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Prefix'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_messaging_prefix', ''),
  //  '#description'    => t('Prefix'),
  //);
  //$form['multiple_base']['messaging']['d4os_io_db_multiple_messaging_username'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Username'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_messaging_username', 'root'),
  //  '#description'    => t('Username'),
  //);
  //$form['multiple_base']['messaging']['d4os_io_db_multiple_messaging_password'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Password'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_messaging_password', 'root'),
  //  '#description'    => t('Password'),
  //);
// opensim
  //$form['multiple_base']['opensim'] = array(
  //  '#type'           => 'fieldset',
  //  '#title'          => t('Opensim'),
  //  '#collapsible'    => TRUE,
  //  '#collapsed'      => TRUE,
  //);
  //$options = $bases;
  //unset($options['os_opensim']);
  //$form['multiple_base']['opensim']['d4os_io_db_multiple_opensim_same'] = array(
  //  '#type'           => 'select',
  //  '#title'          => t('Same as'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_opensim_same', 'none'),
  //  '#options'        => $options,
  //  '#description'    => t('Select if this database is the same as another one.'),
  //);
  //$form['multiple_base']['opensim']['d4os_io_db_multiple_opensim_host'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Host'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_opensim_host', 'localhost'),
  //  '#description'    => t('Host'),
  //);
  //$form['multiple_base']['opensim']['d4os_io_db_multiple_opensim_db'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Database'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_opensim_db', 'opensim'),
  //  '#description'    => t('Database'),
  //);
  //$form['multiple_base']['opensim']['d4os_io_db_multiple_opensim_prefix'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Prefix'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_opensim_prefix', ''),
  //  '#description'    => t('Prefix'),
  //);
  //$form['multiple_base']['opensim']['d4os_io_db_multiple_opensim_username'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Username'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_opensim_username', 'root'),
  //  '#description'    => t('Username'),
  //);
  //$form['multiple_base']['opensim']['d4os_io_db_multiple_opensim_password'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Password'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_opensim_password', 'root'),
  //  '#description'    => t('Password'),
  //);
// profile
  if (module_exists('d4os_io_services_profile')) {
    $form['multiple_base']['profile'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Profile'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $options = $bases;
    unset($options['os_profile']);
    $form['multiple_base']['profile']['d4os_io_db_multiple_profile_same'] = array(
      '#type'           => 'select',
      '#title'          => t('Same as'),
      '#default_value'  => variable_get('d4os_io_db_multiple_profile_same', 'none'),
      '#options'        => $options,
      '#description'    => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['profile']['d4os_io_db_multiple_profile_host'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Host'),
      '#default_value'  => variable_get('d4os_io_db_multiple_profile_host', 'localhost'),
      '#description'    => t('Host'),
    );
    $form['multiple_base']['profile']['d4os_io_db_multiple_profile_db'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Database'),
      '#default_value'  => variable_get('d4os_io_db_multiple_profile_db', 'profile'),
      '#description'    => t('Database'),
    );
    $form['multiple_base']['profile']['d4os_io_db_multiple_profile_prefix'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Prefix'),
      '#default_value'  => variable_get('d4os_io_db_multiple_profile_prefix', ''),
      '#description'    => t('Prefix'),
    );
    $form['multiple_base']['profile']['d4os_io_db_multiple_profile_username'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Username'),
      '#default_value'  => variable_get('d4os_io_db_multiple_profile_username', 'root'),
      '#description'    => t('Username'),
    );
    $form['multiple_base']['profile']['d4os_io_db_multiple_profile_password'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Password'),
      '#default_value'  => variable_get('d4os_io_db_multiple_profile_password', 'root'),
      '#description'    => t('Password'),
    );
  }
// search
  if (module_exists('d4os_io_services_search')) {
    $form['multiple_base']['search'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Search'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $options = $bases;
    unset($options['os_search']);
    $form['multiple_base']['search']['d4os_io_db_multiple_search_same'] = array(
      '#type'           => 'select',
      '#title'          => t('Same as'),
      '#default_value'  => variable_get('d4os_io_db_multiple_search_same', 'none'),
      '#options'        => $options,
      '#description'    => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['search']['d4os_io_db_multiple_search_host'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Host'),
      '#default_value'  => variable_get('d4os_io_db_multiple_search_host', 'localhost'),
      '#description'    => t('Host'),
    );
    $form['multiple_base']['search']['d4os_io_db_multiple_search_db'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Database'),
      '#default_value'  => variable_get('d4os_io_db_multiple_search_db', 'search'),
      '#description'    => t('Database'),
    );
    $form['multiple_base']['search']['d4os_io_db_multiple_search_prefix'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Prefix'),
      '#default_value'  => variable_get('d4os_io_db_multiple_search_prefix', ''),
      '#description'    => t('Prefix'),
    );
    $form['multiple_base']['search']['d4os_io_db_multiple_search_username'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Username'),
      '#default_value'  => variable_get('d4os_io_db_multiple_search_username', 'root'),
      '#description'    => t('Username'),
    );
    $form['multiple_base']['search']['d4os_io_db_multiple_search_password'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Password'),
      '#default_value'  => variable_get('d4os_io_db_multiple_search_password', 'root'),
      '#description'    => t('Password'),
    );
  }
// user_account
  //$form['multiple_base']['user_account'] = array(
  //  '#type'           => 'fieldset',
  //  '#title'          => t('User account'),
  //  '#collapsible'    => TRUE,
  //  '#collapsed'      => TRUE,
  //);
  //$options = $bases;
  //unset($options['os_user_account']);
  //$form['multiple_base']['user_account']['d4os_io_db_multiple_user_account_same'] = array(
  //  '#type'           => 'select',
  //  '#title'          => t('Same as'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_user_account_same', 'none'),
  //  '#options'        => $options,
  //  '#description'    => t('Select if this database is the same as another one.'),
  //);
  //$form['multiple_base']['user_account']['d4os_io_db_multiple_user_account_host'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Host'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_user_account_host', 'localhost'),
  //  '#description'    => t('Host'),
  //);
  //$form['multiple_base']['user_account']['d4os_io_db_multiple_user_account_db'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Database'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_user_account_db', 'user_account'),
  //  '#description'    => t('Database'),
  //);
  //$form['multiple_base']['user_account']['d4os_io_db_multiple_user_account_prefix'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Prefix'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_user_account_prefix', ''),
  //  '#description'    => t('Prefix'),
  //);
  //$form['multiple_base']['user_account']['d4os_io_db_multiple_user_account_username'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Username'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_user_account_username', 'root'),
  //  '#description'    => t('Username'),
  //);
  //$form['multiple_base']['user_account']['d4os_io_db_multiple_user_account_password'] = array(
  //  '#type'           => 'textfield',
  //  '#title'          => t('Password'),
  //  '#default_value'  => variable_get('d4os_io_db_multiple_user_account_password', 'root'),
  //  '#description'    => t('Password'),
  //);
// users
  if (module_exists('d4os_ui_users')) {
    $form['multiple_base']['users'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Users'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $options = $bases;
    unset($options['os_users']);
    $form['multiple_base']['users']['d4os_io_db_multiple_users_same'] = array(
      '#type'           => 'select',
      '#title'          => t('Same as'),
      '#default_value'  => variable_get('d4os_io_db_multiple_users_same', 'none'),
      '#options'        => $options,
      '#description'    => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['users']['d4os_io_db_multiple_users_host'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Host'),
      '#default_value'  => variable_get('d4os_io_db_multiple_users_host', 'localhost'),
      '#description'    => t('Host'),
    );
    $form['multiple_base']['users']['d4os_io_db_multiple_users_db'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Database'),
      '#default_value'  => variable_get('d4os_io_db_multiple_users_db', 'users'),
      '#description'    => t('Database'),
    );
    $form['multiple_base']['users']['d4os_io_db_multiple_users_prefix'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Prefix'),
      '#default_value'  => variable_get('d4os_io_db_multiple_users_prefix', ''),
      '#description'    => t('Prefix'),
    );
    $form['multiple_base']['users']['d4os_io_db_multiple_users_username'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Username'),
      '#default_value'  => variable_get('d4os_io_db_multiple_users_username', 'root'),
      '#description'    => t('Username'),
    );
    $form['multiple_base']['users']['d4os_io_db_multiple_users_password'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Password'),
      '#default_value'  => variable_get('d4os_io_db_multiple_users_password', 'root'),
      '#description'    => t('Password'),
    );
  }

// offline message
  if (module_exists('d4os_io_services_offline_message')) {
    $form['multiple_base']['offline_message'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Offline messages'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $options = $bases;
    unset($options['os_offline_message']);
    $form['multiple_base']['offline_message']['d4os_io_db_multiple_offline_message_same'] = array(
      '#type'           => 'select',
      '#title'          => t('Same as'),
      '#default_value'  => variable_get('d4os_io_db_multiple_offline_message_same', 'none'),
      '#options'        => $options,
      '#description'    => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['offline_message']['d4os_io_db_multiple_offline_message_host'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Host'),
      '#default_value'  => variable_get('d4os_io_db_multiple_offline_message_host', 'localhost'),
      '#description'    => t('Host'),
    );
    $form['multiple_base']['offline_message']['d4os_io_db_multiple_offline_message_db'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Database'),
      '#default_value'  => variable_get('d4os_io_db_multiple_offline_message_db', 'offline_message'),
      '#description'    => t('Database'),
    );
    $form['multiple_base']['offline_message']['d4os_io_db_multiple_offline_message_prefix'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Prefix'),
      '#default_value'  => variable_get('d4os_io_db_multiple_offline_message_prefix', ''),
      '#description'    => t('Prefix'),
    );
    $form['multiple_base']['offline_message']['d4os_io_db_multiple_offline_message_username'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Username'),
      '#default_value'  => variable_get('d4os_io_db_multiple_offline_message_username', 'root'),
      '#description'    => t('Username'),
    );
    $form['multiple_base']['offline_message']['d4os_io_db_multiple_offline_message_password'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Password'),
      '#default_value'  => variable_get('d4os_io_db_multiple_offline_message_password', 'root'),
      '#description'    => t('Password'),
    );
  }
  
  return system_settings_form($form);
}
