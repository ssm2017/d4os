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

function d4os_admin() {
  global $base_url;
  $form = array();

  $form['d4os_uri'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Default URI'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  // grid
  $form['d4os_uri']['grid'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Grid'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  ); // TODO :maybe remove this default grid uri and port
  $form['d4os_uri']['grid']['d4os_default_grid_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default grid URI'),
    '#default_value'  => variable_get('d4os_default_grid_uri', $base_url),
    '#description'    => t("Default grid URI"),
    '#required'       => TRUE,
  );
  $form['d4os_uri']['grid']['d4os_default_grid_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default grid port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_grid_port', '9000'),
    '#description'    => t("Default grid port"),
    '#required'       => TRUE,
  );
  $form['d4os_uri']['grid']['loginuri'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('LoginURI'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['d4os_uri']['grid']['loginuri']['d4os_default_login_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default login URI'),
    '#default_value'  => variable_get('d4os_default_login_uri', $base_url),
    '#description'    => t("Default login URI"),
    '#required'       => TRUE,
  );
  $form['d4os_uri']['grid']['loginuri']['d4os_default_login_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default login port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_login_port', '8002'),
    '#description'    => t("Default login port"),
    '#required'       => TRUE,
  );
  
  // inventory
  $form['d4os_uri']['inventory'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Inventory'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['d4os_uri']['inventory']['d4os_default_inventory_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default inventory URI'),
    '#default_value'  => variable_get('d4os_default_inventory_server_uri', $base_url),
    '#description'    => t("Default inventory URI"),
    '#required'       => TRUE,
  );
  $form['d4os_uri']['inventory']['d4os_default_inventory_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default inventory port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_inventory_server_port', '8004'),
    '#description'    => t("Default inventory port"),
    '#required'       => TRUE,
  );
  // asset
  $form['d4os_uri']['asset'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Asset'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['d4os_uri']['asset']['d4os_default_asset_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default asset URI'),
    '#default_value'  => variable_get('d4os_default_asset_server_uri', $base_url),
    '#description'    => t("Default asset URI"),
    '#required'       => TRUE,
  );
  $form['d4os_uri']['asset']['d4os_default_asset_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default asset port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_asset_server_port', '8003'),
    '#description'    => t("Default asset port"),
    '#required'       => TRUE,
  );
  // asset pictures
  $form['d4os_uri']['pictures'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Pictures'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['d4os_uri']['pictures']['d4os_default_asset_pictures_server_url'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default asset server URL'),
    '#default_value'  => variable_get('d4os_default_asset_pictures_server_url', $base_url. '/asset.php?id='),
    '#description'    => t("Default asset server URL"),
    '#required'       => TRUE,
  );
  return system_settings_form($form);
}

function d4os_admin_io() {
  return t('Use these tabs to configure the io settings');
}

function d4os_admin_services() {
  return t('Use these tabs to configure the services settings');
}

function d4os_admin_ui() {
  return t('Use these tabs to configure the ui settings');
}