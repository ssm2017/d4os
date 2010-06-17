var status_timer = Drupal.settings.d4os_ui_grid_monitor_grid_status.timer*1000;
$.timer(status_timer, function(timer) {
   $.get('/grid/monitor/grid/status', function(data){
      $('#grid_status').replaceWith(data);
   });
});