<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Panels Search Results'),
  'description' => t('Search Results that works with Views Search blocks'),
  'category' => t('Panels Search'),
  'required context' => new ctools_context_optional(t('Keywords'), 'string'),
  'defaults' => array(
    'view_id' => NULL,
  ),
);

/**
 * Render the custom content type.
 */
function panels_search_panels_search_results_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    $keys = '';
  }
  else {
    $keys = $context->data;
  }

  // Build the content type block.
  $block = new stdClass();
  $view_id = $conf['view_id'];
  $view = panels_search_get_view($view_id);

  $output = '';
  if ($view) {
    $display_id = 'default';
    $field = 'search_api_views_fulltext';
    $view->build();
    $view->exposed_widgets = '';
    $output = $view->preview('default');
  }
  $block->content = $output;
  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function panels_search_panels_search_results_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $views = panels_search_search_views();
  $options = array();
  foreach($views as $key => $view_id) {
    $options[$view_id] = $view_id;
  }

  $form['view_id'] = array(
    '#type' => 'select',
    '#title' => t('View ID'),
    '#options' => $options,
    '#default_value' => $conf['view_id'],
  );
  return $form;
}

/**
 * Submit handler for search form.
 */
function panels_search_panels_search_results_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
