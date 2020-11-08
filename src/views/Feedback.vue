<template>
  <v-container class="mt-1">
    <template v-if="!this.$store.state.notification">
      <div class="relative">
        <div class="vertical-center">
          <img
            alt="Jintarkop"
            class="main-logo"
            src="../assets/empty-data.svg"
          />
          <h3>Oops! Data kosong...</h3>
        </div>
      </div>
    </template>
  </v-container>
</template>

<script>
export default {
  mounted() {},
  data: () => ({
    progressColor: {
      brown: "#FDAE20",
      blue: "#108fca"
    },
    notifications: {}
  }),
  computed: {},
  methods: {}
};
</script>

<style scoped>
.icon {
  height: 30px;
}
.main-logo {
  height: 290px;
}
.relative {
  height: 80vh;
  position: relative;
  text-align: center;
}
</style>

<style>
.vertical-center {
  margin: 0;
  position: absolute;
  top: 50%;
  -ms-transform: translateY(-50%);
  transform: translateY(-50%) translateX(-50%);
  left: 50%;
}
</style>