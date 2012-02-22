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

  $form['ugaim'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('UGAIM'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
// user
  $form['ugaim']['user'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Users'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['ugaim']['user']['d4os_default_users_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default users server URI'),
    '#default_value'  => variable_get('d4os_default_users_server_uri', $base_url),
    '#description'    => t("Default users server URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['user']['d4os_default_users_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default users server port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_users_server_port', '8002'),
    '#description'    => t("Default users server port"),
    '#required'       => TRUE,
  );
// grid
  $form['ugaim']['grid'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Grid'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  ); // TODO :maybe remove this default grid uri and port
  $form['ugaim']['grid']['d4os_default_grid_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default grid server URI'),
    '#default_value'  => variable_get('d4os_default_grid_uri', $base_url),
    '#description'    => t("Default grid server URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['grid']['d4os_default_grid_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default grid server port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_grid_port', '9000'),
    '#description'    => t("Default grid server port"),
    '#required'       => TRUE,
  );
// loginuri
  $form['other']['loginuri'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('LoginURI'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['other']['loginuri']['d4os_default_login_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default login URI'),
    '#default_value'  => variable_get('d4os_default_login_uri', $base_url),
    '#description'    => t("Default login URI"),
    '#required'       => TRUE,
  );
  $form['other']['loginuri']['d4os_default_login_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default login port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_login_port', '8002'),
    '#description'    => t("Default login port"),
    '#required'       => TRUE,
  );
// asset pictures
  $form['other']['pictures'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Pictures'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['other']['pictures']['d4os_default_asset_pictures_server_url'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default asset pictures server URL'),
    '#default_value'  => variable_get('d4os_default_asset_pictures_server_url', $base_url. '/asset.php?id='),
    '#description'    => t("Default asset pictures server URL. This server is rendering assets to images."),
    '#required'       => TRUE,
  );
// logging
  $form['other']['debug']['d4os_default_debug_file_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Debugging enabled ?'),
    '#default_value' => variable_get('d4os_default_debug_file_path', ''),
    '#description'   => t('Enable debugging output of server/client interactions. Enter the full absolute path name of the file to write the debugging output to, such as /tmp/d4os.log. It must be writable to the user who runs your web server. Leave blank for no debugging'),
  );
  return system_settings_form($form);
}

/**
 * Admin panel
 */
function d4os_io_settings() {
  $form = array();

  // get available plugins
  $directory = drupal_get_path('module', 'd4os'). '/io/connectors';
	$mask = '.plugin.inc';
	$nomask = array('.', '..', 'CVS');
	$callback = 0;
	$recurse = true;
	$files = file_scan_directory($directory, $mask, $nomask, $callback, $recurse);
  foreach ($files as $file) {
    $name = strstr($file->basename, $mask, TRUE);
  	$options[$name] = $name;
  }
  $form['d4os_io_system'] = array(
    '#type'           => 'radios',
    '#title'          => t('Select io system'),
    '#default_value'  => variable_get('d4os_io_system', 'none'),
    '#options'        => $options,
    '#description'    => t('Select what system you would like to use for input/output.'),
  );
  if (variable_get('d4os_io_system', 'none') != 'none') {
  	module_load_include('admin.inc', 'd4os', '/io/connectors/'. D4OS_IO_SYSTEM. '/'. D4OS_IO_SYSTEM);
  	$plugin_form = 'd4os_io_'. D4OS_IO_SYSTEM. '_admin';
  	$plugin_form($form);
  }
  else {
    drupal_set_message(t("You need to select an io system other than 'None'"), 'error');
  }
  return system_settings_form($form);
}

/**
 * Io empty tab
 */
function d4os_admin_io() {
  return t('Use these tabs to configure the io settings');
}

/**
 * Services empty tab
 */
function d4os_admin_services() {
  return t('Use these tabs to configure the services settings');
}

/**
 * Ui empty tab
 */
function d4os_admin_ui() {
  return t('Use these tabs to configure the ui settings');
}

/**
 * Grid monitor empty tab
 */
function d4os_admin_ui_grid_monitor() {
  return t('Use these tabs to configure the ui grid monitor settings');
}
