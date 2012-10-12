<?php

/**
 * @package    d4os_io db 070
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * @file Db070 admin options.
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
module_load_include('settings.inc', 'd4os', 'io/db_070/db_070');

/**
 * Admin panel
 */
function d4os_io_db_070_admin(&$form) {
  $bases = d4os_io_db_070_build_bases_list();

  $form['d4os_io_db_070_type'] = array(
    '#type' => 'radios',
    '#title' => t('Database system'),
    '#options' => array(t('Same as website'), t('Single'), t('Multiple')),
    '#default_value' => variable_get('d4os_io_db_070_type', 0),
    '#description' => t('Select if you are using only one database for all the OpenSim grid or if you are using separated bases.'),
  );
// single
  $form['single_base'] = array(
    '#type' => 'fieldset',
    '#title' => t('Single base'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Fill these values if you are using a single database.'),
  );
  $form['single_base']['d4os_io_db_070_single_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('d4os_io_db_070_single_host', 'localhost'),
    '#description' => t('Host'),
  );
  $form['single_base']['d4os_io_db_070_single_db'] = array(
    '#type' => 'textfield',
    '#title' => t('Database'),
    '#default_value' => variable_get('d4os_io_db_070_single_db', 'opensim'),
    '#description' => t('Database'),
  );
  $form['single_base']['d4os_io_db_070_single_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => variable_get('d4os_io_db_070_single_prefix', ''),
    '#description' => t('Prefix'),
  );
  $form['single_base']['d4os_io_db_070_single_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('d4os_io_db_070_single_username', 'root'),
    '#description' => t('Username'),
  );
  $form['single_base']['d4os_io_db_070_single_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('d4os_io_db_070_single_password', 'root'),
    '#description' => t('Password'),
  );

// multiple
  $form['multiple_base'] = array(
    '#type' => 'fieldset',
    '#title' => t('Multiple bases'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Fill these values if you are using multiple databases.'),
  );

// robust
  $form['multiple_base']['robust'] = array(
    '#type' => 'fieldset',
    '#title' => t('Robust'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = $bases;
  unset($options['os_robust']);
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_same'] = array(
    '#type' => 'select',
    '#title' => t('Same as'),
    '#default_value' => variable_get('d4os_io_db_070_multiple_robust_same', 'none'),
    '#options' => $options,
    '#description' => t('Select if this database is the same as another one.'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('d4os_io_db_070_multiple_robust_host', 'localhost'),
    '#description' => t('Host'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_db'] = array(
    '#type' => 'textfield',
    '#title' => t('Database'),
    '#default_value' => variable_get('d4os_io_db_070_multiple_robust_db', 'robust'),
    '#description' => t('Database'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => variable_get('d4os_io_db_070_multiple_robust_prefix', ''),
    '#description' => t('Prefix'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('d4os_io_db_070_multiple_robust_username', 'root'),
    '#description' => t('Username'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('d4os_io_db_070_multiple_robust_password', 'root'),
    '#description' => t('Password'),
  );

  if (module_exists('d4os_io_profiles')) {
    // profile
    $form['multiple_base']['profile'] = array(
      '#type' => 'fieldset',
      '#title' => t('Profile'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $options = $bases;
    unset($options['os_profile']);
    $form['multiple_base']['profile']['d4os_io_db_070_multiple_profile_same'] = array(
      '#type' => 'select',
      '#title' => t('Same as'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_profile_same', 'none'),
      '#options' => $options,
      '#description' => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['profile']['d4os_io_db_070_multiple_profile_host'] = array(
      '#type' => 'textfield',
      '#title' => t('Host'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_profile_host', 'localhost'),
      '#description' => t('Host'),
    );
    $form['multiple_base']['profile']['d4os_io_db_070_multiple_profile_db'] = array(
      '#type' => 'textfield',
      '#title' => t('Database'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_profile_db', 'profile'),
      '#description' => t('Database'),
    );
    $form['multiple_base']['profile']['d4os_io_db_070_multiple_profile_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_profile_prefix', ''),
      '#description' => t('Prefix'),
    );
    $form['multiple_base']['profile']['d4os_io_db_070_multiple_profile_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_profile_username', 'root'),
      '#description' => t('Username'),
    );
    $form['multiple_base']['profile']['d4os_io_db_070_multiple_profile_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_profile_password', 'root'),
      '#description' => t('Password'),
    );
  }

  if (module_exists('d4os_io_groups')) {
    // groups
    $form['multiple_base']['groups'] = array(
      '#type' => 'fieldset',
      '#title' => t('groups'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $options = $bases;
    unset($options['os_groups']);
    $form['multiple_base']['groups']['d4os_io_db_070_multiple_groups_same'] = array(
      '#type' => 'select',
      '#title' => t('Same as'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_groups_same', 'none'),
      '#options' => $options,
      '#description' => t('Select if this database is the same as another one.'),
    );
    $form['multiple_base']['groups']['d4os_io_db_070_multiple_groups_host'] = array(
      '#type' => 'textfield',
      '#title' => t('Host'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_groups_host', 'localhost'),
      '#description' => t('Host'),
    );
    $form['multiple_base']['groups']['d4os_io_db_070_multiple_groups_db'] = array(
      '#type' => 'textfield',
      '#title' => t('Database'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_groups_db', 'groups'),
      '#description' => t('Database'),
    );
    $form['multiple_base']['groups']['d4os_io_db_070_multiple_groups_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_groups_prefix', ''),
      '#description' => t('Prefix'),
    );
    $form['multiple_base']['groups']['d4os_io_db_070_multiple_groups_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_groups_username', 'root'),
      '#description' => t('Username'),
    );
    $form['multiple_base']['groups']['d4os_io_db_070_multiple_groups_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#default_value' => variable_get('d4os_io_db_070_multiple_groups_password', 'root'),
      '#description' => t('Password'),
    );
  }
}
