<template>
  <v-container class="px-8 mt-5">
    <template v-if="!this.$store.state.user">
      <div class="relative">
        <div class="vertical-center">
          <img alt="Logo" class="main-logo" src="../assets/empty-data.svg" />
          <h3>Oops! Data kosong...</h3>
        </div>
      </div>
    </template>
    <v-form ref="form" @submit.prevent="submit" lazy-validation>
      <v-text-field
        v-model="feedback.title"
        :rules="isFilled"
        name="title"
        type="text"
        color="#3076BD"
        label="Judul"
      ></v-text-field>
      <v-textarea
        v-model="feedback.body"
        :rules="isFilled"
        name="body"
        label="Isi Feedback"
        auto-grow
      ></v-textarea>

      <v-btn block dark large color="#3076BD" class="mt-5" type="submit"
        >Kirim</v-btn
      >
    </v-form>
    <v-dialog v-model="dialog" persistent max-width="290">
      <v-card>
        <v-card-title class>Apakah data tersebut sudah benar?</v-card-title>
        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="orange darken-1" text @click="dialog = false"
            >Perbaiki</v-btn
          >
          <v-btn color="blue darken-1" text @click="agreeSubmit"
            >Lanjutkan</v-btn
          >
        </v-card-actions>
      </v-card>
    </v-dialog>
  </v-container>
</template>

<script>
export default {
  mounted() {},
  data: () => ({
    feedback: {
      title: "",
      body: ""
    },
    dialog: false
  }),
  computed: {},
  methods: {
    submit() {
      if (this.$refs.form.validate()) {
        this.dialog = true;
      }
    },
    agreeSubmit() {
      this.$store.dispatch("sendFeedback", this.feedback);
      this.dialog = false;
    }
  }
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