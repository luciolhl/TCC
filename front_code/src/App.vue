<template>
  <v-app>
    <v-row class="teste">
      <v-col cols="3">
        <SideBar />
      </v-col>
      <v-col cols="9">
        <v-container>
          <router-view></router-view>
        </v-container>
      </v-col>
    </v-row>
  </v-app>
</template>

<script>
import SideBar from '../src/components/side-bar.vue'
export default {
  name: 'App',
  components: {
    SideBar
  },
  data: () => ({
    //
  }),
};
</script>

<style scoped>
.teste {
  height: 100vh !important;
}
</style>
