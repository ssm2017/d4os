<?php
/**
 * @package    d4os_ui_grid_monitor
 * @subpackage regions_poll
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
function d4os_ui_grid_monitor_regions_poll_admin() {
  $form = array();

  $form['poll_bash'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Bash options'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['poll_bash']['d4os_ui_grid_monitor_regions_poll_bash_hosts_qty'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Hosts quantity'),
    '#default_value'  => variable_get('d4os_ui_grid_monitor_regions_poll_bash_hosts_qty', 5),
    '#size'           => 3,
    '#maxlength'      => 3,
    '#description'    => t("Quantity of hosts to check in one pass."),
    '#required'       => TRUE,
  );
  $form['poll_bash']['d4os_ui_grid_monitor_regions_poll_bash_page_cache_time'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Cache time'),
    '#default_value'  => variable_get('d4os_ui_grid_monitor_regions_poll_bash_page_cache_time', 3600),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#description'    => t("Cache time in seconds to keep results in memory."),
    '#required'       => TRUE,
  );

  return system_settings_form($form);
}
