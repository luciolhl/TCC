'use strict'
const { Model } = require('sequelize')
module.exports = (sequelize, DataTypes) => {
  class Tags extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      Tags.belongsTo(models.Categorias,{
        foreignKey: 'categoria_id'
      } )
    }
  }
  Tags.init(
    {
      // id: {
      //   type: DataTypes.INTEGER,
      //   // validate: {
      //   //   funcaoValidadora: function (dado) {
      //   //     if (dado.length < 3)
      //   //       throw new Error('O campo nome deve ter mais de 3 caracteres!')
      //   //   }
      //   // }
      // },
      local: DataTypes.INTEGER,
    },
    {
      sequelize,
      modelName: 'Tags',
      paranoid: true,
      defaultScope: {
        where: { }
      },
      // scopes: {
      //   todos: { where: {} }
      //   //etc: {constraint: valor}
      // }
    }
  )
  return Tags
}
