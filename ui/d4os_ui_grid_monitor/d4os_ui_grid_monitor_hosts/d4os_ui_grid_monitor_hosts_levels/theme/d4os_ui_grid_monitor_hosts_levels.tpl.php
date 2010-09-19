<?php
/**
 * @package    d4os_ui_grid_monitor
 * @subpackage hosts_levels
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
?>
<div class="item-list">
  <ul>
    <li class="first">
      <div class="form-item" id="host-wrapper">
        <label for="host"><?php print t('Host'); ?></label>
        <div id="host"><?php print $data->host; ?></div>
      </div>
    </li>
    <li class="form-item">
      <div class="form-item" id="port-wrapper">
        <label for="port"><?php print t('Port'); ?></label>
        <div id="port"><?php print $data->port; ?></div>
      </div>
    </li>
    <li>
      <div class="form-item" id="par-version-wrapper">
        <label for="par-version"><?php print t('Version'); ?></label>
        <div id="par-version"><?php print $data->Version; ?></div>
      </div>
    </li>
    <li>
      <div class="form-item" id="par-uptime-wrapper">
        <label for="par-uptime"><?php print t('Uptime'); ?></label>
        <div id="par-uptime"><?php print $data->Uptime; ?></div>
      </div>
    </li>
    <li>
      <div class="form-item" id="par-ragent-wrapper">
        <label for="par-ragent"><?php print t('Root agent'); ?></label>
        <div id="par-ragent"><?php print $data->RootAg; ?></div>
      </div>
    </li>
    <li class="last">
      <div class="form-item" id="par-cagent-wrapper">
        <label for="par-cagent"><?php print t('Child agent'); ?></label>
        <div id="par-cagent"><?php print $data->ChldAg; ?></div>
      </div>
    </li>
  </ul>
  <?php print $widget; ?>
</div>
