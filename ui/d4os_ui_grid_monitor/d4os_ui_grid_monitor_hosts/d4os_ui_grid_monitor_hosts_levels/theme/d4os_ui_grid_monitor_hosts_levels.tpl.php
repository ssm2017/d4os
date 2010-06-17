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