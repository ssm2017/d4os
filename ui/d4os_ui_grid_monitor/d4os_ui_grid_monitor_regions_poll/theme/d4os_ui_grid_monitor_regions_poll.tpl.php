<h1><?php print t("Grid regions VS Search regions"); ?></h1>
<h2><?php print t("Initial count"); ?></h2>
<p><?php print t("There are !regions regions in the grid.", array('!regions'=>$data['grid_regions_qty'])); ?></p>
<p><?php print t("There are !regions regions in the search.", array('!regions'=>$data['search_regions_qty'])); ?></p>
<h2><?php print t("Comparison"); ?></h2>
<p><?php print t("There are !regions regions in the grid but not in the search", array('!regions'=>$data['diff_grid_vs_search_qty'])); ?></p>
<p><?php print t("There are !regions regions in the search but not in the grid", array('!regions'=>$data['diff_search_vs_grid_qty'])); ?></p>
<h2><?php print t("Grid regions analysis"); ?></h2>
<p><?php print t("There are !hosts hosts.", array('!hosts'=>$data['hosts_qty'])); ?></p>
<?php if (count($data['hosts'])): ?>
<p><?php print t("In these hosts, there are !offline hosts not responding and !online hosts responding to /simstatus/", array('!offline'=>$data['offline_hosts_qty'], '!online'=>$data['online_hosts_qty'])); ?></p>
<?php if (count($data['online_hosts'])): ?>
<h3><?php print t("Online hosts"); ?></h3>
<table>
  <thead>
    <th><?php print t("collector ?"); ?></th>
    <th><?php print t("Host Uri"); ?></th>
    <th><?php print t("Region name"); ?></th>
    <th><?php print t("Region owner"); ?></th>
  </thead>
  <tbody>
    <?php foreach ($data['online_hosts'] as $online_host): ?>
    <tr>
      <td><?php print ($online_host['collector']) ? "<strong>". t("Yes"). "</strong>": "&nbsp"; ?></td>
      <td><?php print $online_host['host']; ?></td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
    </tr>
    <?php foreach ($online_host['regions'] as $region): ?>
    <tr>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td><?php print $region->regionName; ?></td>
      <td><?php print $region->owner; ?></td>
    </tr>
    <?php endforeach; // region ?>
    <?php endforeach; // online_host ?>
  </tbody>
</table>
<?php endif; // online_hosts ?>
<?php if (count($data['offline_hosts'])): ?>
<h3><?php print t("Offline hosts"); ?></h3>
<table>
  <thead>
    <th><?php print t("Host Uri"); ?></th>
    <th><?php print t("Region name"); ?></th>
    <th><?php print t("Region owner"); ?></th>
  </thead>
  <tbody>
    <?php foreach ($data['offline_hosts'] as $offline_host): ?>
    <tr>
      <td><?php print $offline_host['host']; ?></td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
    </tr>
    <?php foreach ($offline_host['regions'] as $region): ?>
    <tr>
      <td>&nbsp;</td>
      <td><?php print $region->regionName; ?></td>
      <td><?php print $region->owner; ?></td>
    </tr>
    <?php endforeach; // region ?>
    <?php endforeach; // offline_host ?>
  </tbody>
</table>
<?php endif; // offline_hosts ?>
<?php endif; // hosts ?>
