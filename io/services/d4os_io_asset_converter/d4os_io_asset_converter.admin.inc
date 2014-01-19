<?php

/**
 * @package    d4os_io_asset_converter
 * @copyright Copyright (C) 2014 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * d4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_io_asset_converter_admin() {
  global $base_url;
  $form = array();
// asset server
  $form['asset_server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Asset server'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['asset_server']['d4os_io_asset_converter_asset_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Asset server URL'),
    '#default_value' => variable_get('d4os_io_asset_converter_asset_server_url', $base_url . '/asset.php?id='),
    '#description' => t("The asset server that you will use."),
    '#required' => TRUE,
  );
// local files
  $form['local_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local files'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['local_files']['d4os_io_asset_converter_cache_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect'),
    '#options' => array(
      0 => 'Direct',
      1 => 'Redirect'
    ),
    '#default_value' => variable_get('d4os_io_asset_converter_cache_redirect', 1),
    '#description' => t("If the file exists in the cache, redirect the browser directly to the file.")
  );
  $form['local_files']['d4os_io_asset_converter_cache_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache folder'),
    '#default_value' => variable_get('d4os_io_asset_converter_cache_folder', 'assets'),
    '#description' => t("The folder to store the cache.(this is relative to Drupal public folder.)"),
    '#required' => TRUE,
  );
  $form['local_files']['d4os_io_asset_converter_cache_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache max lifetime'),
    '#default_value' => variable_get('d4os_io_asset_converter_cache_lifetime', 86400),
    '#description' => t("The value is in seconds."),
    '#required' => TRUE,
  );
// images
  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['images']['d4os_io_asset_converter_image_default_extension'] = array(
    '#type' => 'select',
    '#title' => t('Default image extension'),
    '#options' => array(
      'jpg' => 'JPG',
      'png' => 'PNG',
      'gif' => 'GIF'
    ),
    '#default_value' => variable_get('d4os_io_asset_converter_image_default_extension', 'jpg'),
    '#description' => t("The default image format."),
    '#required' => TRUE,
  );
  $form['images']['d4os_io_asset_converter_image_default_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Default width'),
    '#default_value' => variable_get('d4os_io_asset_converter_image_default_width', 512),
    '#description' => t("The value is in pixels."),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
