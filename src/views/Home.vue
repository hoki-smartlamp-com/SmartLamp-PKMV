<template>
  <v-container class="px-8">
    <Admin v-if="userRole.role == 'admin'" />
    <User v-else />
  </v-container>
</template>

<script>
import User from "@/components/Home/User";
import Admin from "@/components/Home/Admin";
export default {
  name: "Home",
  components: {
    Admin,
    User
  },
  data: () => ({}),
  mounted() {},
  computed: {
    userRole() {
      return this.$store.state.user || "user";
    }
  },
  watch: {},
  methods: {}
};
</script>

<style scoped>
.feedback-card {
  height: 42vh;
}
</style>