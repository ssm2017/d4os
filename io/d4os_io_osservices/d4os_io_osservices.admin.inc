<?php
/**
* @package		d4os
* @copyright	Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license		GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_io_osservices_admin() {
  $form = array();
// grid
  $form['d4os_io_osservices_grid'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Grid'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['d4os_io_osservices_grid']['d4os_io_osservices_default_grid_pause'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default grid pause'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_io_osservices_default_grid_pause', 5),
    '#description'    => t("Delay between every call."),
    '#required'       => TRUE,
  );
// uri
  $form['d4os_io_osservices_uri'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Default URI'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['d4os_io_osservices_uri']['d4os_io_osservices_default_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default oss URI'),
    '#default_value'  => variable_get('d4os_io_osservices_default_uri', '127.0.0.1'),
    '#description'    => t("Default oss URI."),
    '#required'       => TRUE,
  );

  $form['d4os_io_osservices_uri']['d4os_io_osservices_default_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default oss port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_io_osservices_default_port', '7999'),
    '#description'    => t("Default oss port."),
    '#required'       => TRUE,
  );
// credentials
  $form['d4os_io_osservices_credentials'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Default credentials'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['d4os_io_osservices_credentials']['d4os_io_osservices_default_user'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default user'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_user', 'admin'),
    '#description'    => t('Default user'),
    '#required'       => TRUE,
  );

  $form['d4os_io_osservices_credentials']['d4os_io_osservices_default_pass'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default password'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_pass', 'f6fdffe48c908deb0f4c3bd36c032e72'),
    '#description'    => t('Default password'),
    '#required'       => TRUE,
  );
// uuid
  $form['d4os_io_osservices_uuid'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Default UUID'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['d4os_io_osservices_uuid']['d4os_io_osservices_default_osmaps_uuid'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default OSMAPS UUID'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_osmaps_uuid', 'a9468cec-e02b-11de-b2b0-0015f297a987'),
    '#description'    => t('Default OSMAPS UUID'),
    '#required'       => TRUE,
  );

  $form['d4os_io_osservices_uuid']['d4os_io_osservices_default_users_uuid'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default USERS UUID'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_users_uuid', 'c70a2070-e028-11de-b2b0-0015f297a987'),
    '#description'    => t('Default USERS UUID'),
    '#required'       => TRUE,
  );

  $form['d4os_io_osservices_uuid']['d4os_io_osservices_default_robust_uuid'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default ROBUST UUID'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_robust_uuid', 'a513750a-e02a-11de-b2b0-0015f297a987'),
    '#description'    => t('Default ROBUST UUID'),
    '#required'       => TRUE,
  );

  $form['d4os_io_osservices_uuid']['d4os_io_osservices_default_messaging_uuid'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default MESSAGING UUID'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_messaging_uuid', 'b4e36656-e02b-11de-b2b0-0015f297a987'),
    '#description'    => t('Default MESSAGING UUID'),
    '#required'       => TRUE,
  );

  $form['d4os_io_osservices_uuid']['d4os_io_osservices_default_opensim_uuid'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default OPENSIM UUID'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_osservices_default_opensim_uuid', 'e978e986-61a9-4d93-a40a-c5e9e05047d0'),
    '#description'    => t('Default OPENSIM UUID'),
    '#required'       => TRUE,
  );

  return system_settings_form($form);
}
