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
 
?>
<style>
  #regions_poll td {
    padding:3px;
    border:1px solid black;
  }
  #regions_poll td.green {
    color:white;
    background-color:green;
  }
  #regions_poll td.or {
    color:white;
    background-color:orange;
  }
  #regions_poll td.red {
    color:white;
    background-color:red;
  }

</style>
<div id="regions_poll">
<h2><?php print t("Hosts"); ?></h2>
<h3><?php print t("Numbers"); ?></h3>
<ul>
  <li><?php print t("Machines"); ?> : <?php print count($data['hosts']['all']['ips']); ?></li>
  <li><?php print t("In grid"); ?> : <?php print count($data['results']['hosts']['in_grid']); ?></li>
  <li><?php print t("In search"); ?> : <?php print count($data['results']['hosts']['in_search']); ?></li>
  <li><?php print t("Registered"); ?> : <?php print count($data['results']['hosts']['registered']); ?></li>
  <li><?php print t("In grid but not in search"); ?> : <?php print count($data['results']['hosts']['in_grid_but_not_in_search']); ?></li>
  <li><?php print t("In search but not in grid"); ?> : <?php print count($data['results']['hosts']['in_search_but_not_in_grid']); ?></li>
  <li><?php print t("In search but not registered"); ?> : <?php print count($data['results']['hosts']['in_search_but_not_registered']); ?></li>
  <li><?php print t("In all"); ?> : <?php print count($data['results']['hosts']['in_all']); ?></li>
  <li><?php print t("Online"); ?> : <?php print count($data['results']['hosts']['online']); ?></li>
  <li><?php print t("Online and in all"); ?> : <?php print count($data['results']['hosts']['online_in_all']); ?></li>
  <li><?php print t("Online, in all, and collector"); ?> : <?php print count($data['results']['hosts']['online_in_all_and_collector']); ?></li>
</ul>
<h3><?php print t("Slow hosts"); ?></h3>
<ul>
  <?php foreach ($data['results']['hosts']['slow_items'] as $host): ?>
  <li><?php print $host; ?></li>
  <?php endforeach; ?>
</ul>
<h3><?php print t("All hosts list"); ?></h3>
<table>
  <caption><?php print t("Hosts status"); ?></caption>
  <thead>
    <th><?php print t("Machine IP"); ?></th>
    <th><?php print t("Host url"); ?></th>
    <th><?php print t("From grid"); ?></th>
    <th><?php print t("In grid"); ?></th>
    <th><?php print t("From search"); ?></th>
    <th><?php print t("In search"); ?></th>
    <th><?php print t("From registered"); ?></th>
    <th><?php print t("Registered"); ?></th>
    <th><?php print t("Is online"); ?></th>
    <th><?php print t("Speed"); ?></th>
    <th><?php print t("Use collector"); ?></th>
    <th><?php print t("Speed"); ?></th>
    <th><?php print t("Region name"); ?></th>
    <th><?php print t("Region uuid"); ?></th>
    <th><?php print t("Region handle"); ?></th>
    <th><?php print t("Region owner"); ?></th>
    <th><?php print t("In grid"); ?></th>
    <th><?php print t("In search"); ?></th>
  </thead>
  <tbody>
    <?php foreach ($data['hosts']['all']['ips'] as $machine): ?>
    <tr>
      <td><?php print $machine; ?></td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
    </tr>
    <?php foreach ($data['hosts']['all']['items'] as $host): ?>
    <?php if ($host['host_ip'] == $machine): ?>
    <tr>
      <td>&nbsp;</td>
      <td><?php print $host['host_domain_url']; ?></td>
      <td class="<?php print ($host['from_grid']) ? 'green': 'white';?>"><?php print ($host['from_grid']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($host['in_grid']) ? 'green': 'white';?>"><?php print ($host['in_grid']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($host['from_search']) ? 'green': 'white';?>"><?php print ($host['from_search']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($host['in_search']) ? 'green': 'white';?>"><?php print ($host['in_search']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($host['from_registered']) ? 'green': 'white';?>"><?php print ($host['from_registered']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($host['registered']) ? 'green': 'white';?>"><?php print ($host['registered']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($host['is_online']) ? 'green': 'white';?>"><?php print ($host['is_online']) ? t('Yes'): t('No');?></td>
      <td class="<?php print $host['time']['simstatus']['color'];?>"><?php print round($host['time']['simstatus']['speed'], 3); ?></td>
      <td class="<?php print ($host['use_collector']) ? 'green': 'white';?>"><?php print ($host['use_collector']) ? t('Yes'): t('No');?></td>
      <td class="<?php print $host['time']['collector']['color'];?>"><?php print round($host['time']['collector']['speed'], 3); ?></td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
    </tr>
    <?php foreach ($data['regions']['all']['items'] as $region): ?>
    <?php if ($region['host_domain_url'] == $host['host_domain_url']): ?>
    <tr>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td><?php print $region['name']; ?></td>
      <td><?php print $region['uuid']; ?></td>
      <td><?php print $region['handle']; ?></td>
      <td><?php print $region['owner_name']; ?></td>
      <td class="<?php print ($region['in_grid']) ? 'green': 'white';?>"><?php print ($region['in_grid']) ? t('Yes'): t('No');?></td>
      <td class="<?php print ($region['in_search']) ? 'green': 'white';?>"><?php print ($region['in_search']) ? t('Yes'): t('No');?></td>
    </tr>
    <?php endif; // if ($region['host_domain_url'] == $host['host_domain_url']): ?>
    <?php endforeach; // region ?>
    <?php endif; // if ($host->host_ip == $machine): ?>
    <?php endforeach; // host ?>
    <?php endforeach; // machine ?>
  </tbody>
</table>
</div>
