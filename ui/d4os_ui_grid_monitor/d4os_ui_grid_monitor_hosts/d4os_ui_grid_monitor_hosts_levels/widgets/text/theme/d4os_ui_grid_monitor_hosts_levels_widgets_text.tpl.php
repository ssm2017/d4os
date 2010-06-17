<div id="d4os-stats-levels-widget">
  <div class="item-list">
    <ul>
      <li class="first">
        <div class="form-item" id="par-simfps-wrapper">
          <label for="par-simfps"><?php print t('SimFPS'); ?></label>
          <div id="par-simfps"><?php print round($data->SimFPS); ?></div>
        </div>
      </li>
      <li>
        <div class="form-item" id="par-phyfps-wrapper">
          <label for="par-phyfps"><?php print t('PhyFPS'); ?></label>
          <div id="par-phyfps"><?php print round($data->PhyFPS); ?></div>
        </div>
      </li>
      <li class="last">
        <div class="form-item" id="par-memory-wrapper">
          <label for="par-memory"><?php print t('Memory'); ?></label>
          <div id="par-memory"><?php print round($data->Memory); ?></div>
        </div>
      </li>
    </ul>
  </div>
</div>