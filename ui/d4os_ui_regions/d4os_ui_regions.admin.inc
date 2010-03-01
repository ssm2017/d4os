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

function d4os_ui_regions_admin() {
  $form = array();

  $form['d4os_ui_regions_list'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Regions list'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );

  $options = array(
    'uuid'              => t('uuid'),
    'regionHandle'      => t('regionHandle'),
    'regionName'        => t('regionName'),
    'serverIP'          => t('serverIP'),
    'serverPort'        => t('serverPort'),
    'serverURI'         => t('serverURI'),
    'locX'              => t('locX'),
    'locY'              => t('locY'),
    'locZ'              => t('locZ'),
    'regionMapTexture'  => t('regionMapTexture'),
    'serverHttpPort'    => t('serverHttpPort'),
    'owner_uuid'        => t('owner_uuid'),
    'sizeX'             => t('sizeX'),
    'sizeY'             => t('sizeY'),
  );
  $form['d4os_ui_regions_list']['d4os_ui_regions_list_items'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Items'),
    '#default_value' => variable_get('d4os_ui_regions_list_items', array('regionName', 'locX', 'locY')),
    '#options' => $options,
    '#description' => t('Select the items you want to display on the list.'),
  );

  return system_settings_form($form);
}
