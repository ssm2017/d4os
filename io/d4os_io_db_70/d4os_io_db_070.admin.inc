<?php
/**
 * @package    d4os_io_db
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_io_db_070_admin() {
  $bases = d4os_io_db_070_build_bases_list();

  $form = array();

  $form['d4os_io_db_070_type'] = array(
    '#type'           => 'radios',
    '#title'          => t('Database system'),
    '#options'        => array(t('Same as website'), t('Single'), t('Multiple')),
    '#default_value'  => variable_get('d4os_io_db_070_type', 0),
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
  $form['single_base']['d4os_io_db_070_single_host'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host'),
    '#default_value'  => variable_get('d4os_io_db_070_single_host', 'localhost'),
    '#description'    => t('Host'),
  );
  $form['single_base']['d4os_io_db_070_single_db'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Database'),
    '#default_value'  => variable_get('d4os_io_db_070_single_db', 'opensim'),
    '#description'    => t('Database'),
  );
  $form['single_base']['d4os_io_db_070_single_prefix'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Prefix'),
    '#default_value'  => variable_get('d4os_io_db_070_single_prefix', ''),
    '#description'    => t('Prefix'),
  );
  $form['single_base']['d4os_io_db_070_single_username'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Username'),
    '#default_value'  => variable_get('d4os_io_db_070_single_username', 'root'),
    '#description'    => t('Username'),
  );
  $form['single_base']['d4os_io_db_070_single_password'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Password'),
    '#default_value'  => variable_get('d4os_io_db_070_single_password', 'root'),
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
  
// robust
  $form['multiple_base']['robust'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('robust'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );
  $options = $bases;
  unset($options['os_robust']);
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_same'] = array(
    '#type'           => 'select',
    '#title'          => t('Same as'),
    '#default_value'  => variable_get('d4os_io_db_070_multiple_robust_same', 'none'),
    '#options'        => $options,
    '#description'    => t('Select if this database is the same as another one.'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_host'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Host'),
    '#default_value'  => variable_get('d4os_io_db_070_multiple_robust_host', 'localhost'),
    '#description'    => t('Host'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_db'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Database'),
    '#default_value'  => variable_get('d4os_io_db_070_multiple_robust_db', 'robust'),
    '#description'    => t('Database'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_prefix'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Prefix'),
    '#default_value'  => variable_get('d4os_io_db_070_multiple_robust_prefix', ''),
    '#description'    => t('Prefix'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_username'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Username'),
    '#default_value'  => variable_get('d4os_io_db_070_multiple_robust_username', 'root'),
    '#description'    => t('Username'),
  );
  $form['multiple_base']['robust']['d4os_io_db_070_multiple_robust_password'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Password'),
    '#default_value'  => variable_get('d4os_io_db_070_multiple_robust_password', 'root'),
    '#description'    => t('Password'),
  );
  
  return system_settings_form($form);
}
