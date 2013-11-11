<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Panels Search Form'),
  'description' => t('Search Form that works with Views Search blocks'),
  'category' => t('Panels Search'),
  'required context' => new ctools_context_optional(t('Keywords'), 'string'),
  'defaults' => array(
    'view_id' => '',
    'search_path' => '',
  ),
);

/**
 * Render the custom content type.
 */
function panels_search_panels_search_form_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    $keys = '';
  }
  else {
    $keys = $context->data;
  }

  $view_id = $conf['view_id'];
  $search_path = isset($conf['search_path']) ? $conf['search_path'] : '';
  // Build the content type block.
  $block = new stdClass();
  $view = panels_search_get_view($view_id);
  if($search_path) {
    $view->override_url = $search_path;
  }

  if ($view) {
    $form_html = panels_search_render_exposed_form($view);
  }
  $block->content = $form_html;
  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function panels_search_panels_search_form_content_type_edit_form($form, &$form_state) {
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

  $form['search_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Path'),
    '#default_value' => $conf['search_path'],
  );
  return $form;
}

/**
 * Submit handler for search form.
 */
function panels_search_panels_search_form_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
