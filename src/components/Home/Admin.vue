<template>
  <div>
    <v-dialog v-model="loading" persistent width="300">
      <v-card color="#3076BD" dark>
        <v-card-text>
          Tolong tunggu sebentar...
          <v-progress-linear
            indeterminate
            color="white"
            class="mb-0"
          ></v-progress-linear>
        </v-card-text>
      </v-card>
    </v-dialog>
    <v-card class="pa-5">
      <v-form ref="form" @submit.prevent="addNewLamp" lazy-validation>
        <h3>Tambah baru kode seri lampu</h3>
        <v-text-field
          label="Kode Seri Lampu"
          name="lamp_id"
          v-model="lamp_id"
          :rules="isFilled"
          required
        ></v-text-field>
        <v-btn class="mt-2" block dark large color="#3076BD" type="submit"
          >Tambahkan</v-btn
        >
      </v-form>
    </v-card>

    <h3 class="mt-5">List Kode Seri Lampu</h3>
    <v-card class="px-5 py-4 mt-1 list-card"
      ><ol>
        <li class="body-1" v-for="(list, i) of lists" :key="i">{{ list }}</li>
      </ol>
    </v-card>
    <v-btn block dark large color="#EA6A00" class="mt-3" to="/list-feedback"
      >Lihat Feedback User</v-btn
    >
  </div>
</template>

<script>
// import Modal from "@/components/Modal";
export default {
  name: "Home",
  components: {},
  data: () => ({
    lists: [],
    lamp_id: ""
  }),
  mounted() {
    this.$store.dispatch("getLampId").then(res => {
      for (let list in res) {
        this.lists.push(list);
      }
    });
  },
  computed: {
    loading() {
      return this.$store.state.loading;
    }
  },
  watch: {},
  methods: {
    addNewLamp() {
      if (this.$refs.form.validate()) {
        this.$store.dispatch("addNewLamp", this.lamp_id);
        this.lamp_id = "";
      }
    }
  }
};
</script>

<style scoped>
.list-card {
  height: 32vh;
  overflow: scroll;
}
</style>