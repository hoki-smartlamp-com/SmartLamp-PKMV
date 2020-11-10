<template>
  <v-container class="px-8">
    <v-card
      class="px-5 pt-3 pb-2 mb-3 feedback-card"
      v-for="(feedback, i) in feedbacks"
      :key="i"
    >
      <h3>{{ feedback.username }}</h3>
      <span>{{ feedback.email }}</span>
      <hr class="my-3" />
      <h4>Judul:</h4>
      <p>{{ feedback.title }}</p>
      <h4>Isi:</h4>
      <p>{{ feedback.body }}</p>
    </v-card>
  </v-container>
</template>

<script>
export default {
  data: () => ({
    feedbacks: {}
  }),
  mounted() {
    this.$store.dispatch("getFeedback").then(res => {
      this.feedbacks = res;
    });
  }
};
</script>

<style>
</style>