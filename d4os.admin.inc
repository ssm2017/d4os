<?php

/**
 * @package   d4os_main
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_admin() {
  global $base_url;
  $form = array();

// grid
  $form['grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grid'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t("Default grid server URI. This uri is used to check if the grid is alive."),
  );
  $form['grid']['d4os_default_grid_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Default grid server URI'),
    '#default_value' => variable_get('d4os_default_grid_uri', $base_url),
    '#description' => t("Default grid server URI."),
    '#required' => TRUE,
  );
  $form['grid']['d4os_default_grid_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Default grid server port'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('d4os_default_grid_port', '9000'),
    '#description' => t("Default grid server port"),
    '#required' => TRUE,
  );
  $form['grid']['d4os_default_grid_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default grid server path'),
    '#default_value' => variable_get('d4os_default_grid_path', ''),
    '#description' => t("Default grid server path. This path will be added at the end of the uri like http://uri:port/path. <br />Note: you must add a preceding forward slash '/'.<br />Leave blank if not needed."),
    '#required' => FALSE,
  );
// loginuri
  $form['loginuri'] = array(
    '#type' => 'fieldset',
    '#title' => t('LoginURI'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['loginuri']['d4os_default_login_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Default login URI'),
    '#default_value' => variable_get('d4os_default_login_uri', $base_url),
    '#description' => t("Default login URI"),
    '#required' => TRUE,
  );
  $form['loginuri']['d4os_default_login_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Default login port'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('d4os_default_login_port', '8002'),
    '#description' => t("Default login port"),
    '#required' => TRUE,
  );
  $form['loginuri']['d4os_default_login_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default login path'),
    '#default_value' => variable_get('d4os_default_login_path', ''),
    '#description' => t("Default login path. This path will be added at the end of the uri like http://uri:port/path. <br />Note: you must add a preceding forward slash '/'.<br />Leave blank if not needed."),
    '#required' => FALSE,
  );
// hguri
  $form['hguri'] = array(
    '#type' => 'fieldset',
    '#title' => t('HG Uri'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['hguri']['d4os_default_hg_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Default login URI'),
    '#default_value' => variable_get('d4os_default_hg_uri', $base_url),
    '#description' => t("Default HG URI"),
    '#required' => TRUE,
  );
  $form['hguri']['d4os_default_hg_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Default HG port'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('d4os_default_hg_port', '8002'),
    '#description' => t("Default HG port"),
    '#required' => TRUE,
  );
// asset pictures
  $form['pictures'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pictures'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['pictures']['d4os_default_asset_pictures_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Default asset pictures server URL'),
    '#default_value' => variable_get('d4os_default_asset_pictures_server_url', $base_url . '/asset.php?id='),
    '#description' => t("Default asset pictures server URL. This server is rendering assets to images."),
    '#required' => TRUE,
  );
// logging
  $form['debug']['d4os_default_debug_file_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Debugging enabled ?'),
    '#default_value' => variable_get('d4os_default_debug_file_path', ''),
    '#description' => t('Enable debugging output of server/client interactions. Enter the full absolute path name of the file to write the debugging output to, such as /tmp/d4os.log. It must be writable to the user who runs your web server. Leave blank for no debugging'),
  );
  return system_settings_form($form);
}

/**
 * Admin panel
 */
function d4os_io_settings() {
  $form = array();

  // get available plugins
  $files = file_scan_directory(drupal_get_path('module', 'd4os') . '/io/connectors', '/.*\.plugin\.inc$/');
  $options = array('none' => 'None');
  foreach ($files as $file) {
    $name = strstr($file->name, '.plugin', TRUE);
    $options[$name] = $name;
  }
  $form['d4os_io_system'] = array(
    '#type' => 'radios',
    '#title' => t('Select io system'),
    '#default_value' => variable_get('d4os_io_system', 'none'),
    '#options' => $options,
    '#description' => t('Select what system you would like to use for input/output.'),
  );

  if (variable_get('d4os_io_system', 'none') != 'none') {
    module_load_include('admin.inc', 'd4os', '/io/connectors/' . D4OS_IO_SYSTEM . '/' . D4OS_IO_SYSTEM);
    $plugin_form = 'd4os_io_' . D4OS_IO_SYSTEM . '_admin';
    $plugin_form($form);
  }
  else {
    drupal_set_message(t("You need to select an io system other than 'None'"), 'error');
  }
  return system_settings_form($form);
}
