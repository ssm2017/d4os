<?php
/**
 * @package    d4os_ui_grid_monitor
 * @subpackage hosts_levels_widgets_text
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Display the result
 */
function d4os_ui_grid_monitor_hosts_levels_widgets_text_display($data) {
  if (variable_get('d4os_ui_grid_monitor_hosts_levels_use_timer', 1)) {
    drupal_add_js(drupal_get_path('module', 'd4os_ui_grid_monitor_hosts_levels').'/widgets/text/js/d4os_ui_grid_monitor_hosts_levels_widgets_text_values.js');
  }
  $output = theme('d4os_ui_grid_monitor_hosts_levels_widgets_text', $data);
  return $output;
}
