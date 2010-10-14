<?php
/**
 * @package   d4os_main
 * @copyright Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
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

  $form['d4os_check_datastore_health'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Check datastore health'),
    '#default_value'  => variable_get('d4os_check_datastore_health', 1),
    '#description'    => t('Use this option to check if the grid datastores are alive before acting some operations like registering users.'),
  );
  $form['ugaim'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('UGAIM'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
// user
  $form['ugaim']['user'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('User'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['ugaim']['user']['d4os_default_user_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default user URI'),
    '#default_value'  => variable_get('d4os_default_user_server_uri', $base_url),
    '#description'    => t("Default user URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['user']['d4os_default_user_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default user port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_user_server_port', '8002'),
    '#description'    => t("Default user port"),
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
    '#title'          => t('Default grid URI'),
    '#default_value'  => variable_get('d4os_default_grid_uri', $base_url),
    '#description'    => t("Default grid URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['grid']['d4os_default_grid_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default grid port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_grid_port', '9000'),
    '#description'    => t("Default grid port"),
    '#required'       => TRUE,
  );
// asset
  $form['ugaim']['asset'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Asset'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['ugaim']['asset']['d4os_default_asset_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default asset URI'),
    '#default_value'  => variable_get('d4os_default_asset_server_uri', $base_url),
    '#description'    => t("Default asset URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['asset']['d4os_default_asset_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default asset port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_asset_server_port', '8003'),
    '#description'    => t("Default asset port"),
    '#required'       => TRUE,
  );
// inventory
  $form['ugaim']['inventory'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Inventory'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['ugaim']['inventory']['d4os_default_inventory_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default inventory URI'),
    '#default_value'  => variable_get('d4os_default_inventory_server_uri', $base_url),
    '#description'    => t("Default inventory URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['inventory']['d4os_default_inventory_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default inventory port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_inventory_server_port', '8003'),
    '#description'    => t("Default inventory port"),
    '#required'       => TRUE,
  );
// messaging
  $form['ugaim']['messaging'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Messaging'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['ugaim']['messaging']['d4os_default_messaging_server_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default messaging URI'),
    '#default_value'  => variable_get('d4os_default_messaging_server_uri', $base_url),
    '#description'    => t("Default messaging URI"),
    '#required'       => TRUE,
  );
  $form['ugaim']['messaging']['d4os_default_messaging_server_port'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default messaging port'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_default_messaging_server_port', '8006'),
    '#description'    => t("Default messaging port"),
    '#required'       => TRUE,
  );
  $form['other'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Other'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
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
    '#title'          => t('Default asset server URL'),
    '#default_value'  => variable_get('d4os_default_asset_pictures_server_url', $base_url. '/asset.php?id='),
    '#description'    => t("Default asset server URL"),
    '#required'       => TRUE,
  );
// logging
  $form['other']['debug']['d4os_default_debug_file_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Debugging enabled ?'),
    '#default_value' => variable_get('d4os_default_debug_file_path', ''),
    '#description'   => t('Enable debugging output of server/client interactions. Enter the full path name of the file to write the debugging output to, such as /tmp/d4os.debug. It must be writable to the user who runs your web server. Leave blank for no debugging'),
  );
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
