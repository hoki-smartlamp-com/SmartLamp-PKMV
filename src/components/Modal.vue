<template>
  <v-row justify="center">
    <v-dialog v-model="dialog" persistent max-width="600px">
      <v-card class="pa-3">
        <slot></slot>
      </v-card>
    </v-dialog>
  </v-row>
</template>

<script>
export default {
  data: () => ({}),
  computed: {
    dialog() {
      return this.$store.state.modalEditColor;
    }
  }
};
</script>

<style>
</style>